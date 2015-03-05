/* XiVO Client
 * Copyright (C) 2007-2014 Avencall
 *
 * This file is part of XiVO Client.
 *
 * XiVO Client is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version, with a Section 7 Additional
 * Permission as follows:
 *   This notice constitutes a grant of such permission as is necessary
 *   to combine or link this software, or a modified version of it, with
 *   the OpenSSL project's "OpenSSL" library, or a derivative work of it,
 *   and to copy, modify, and distribute the resulting work. This is an
 *   extension of the special permission given by Trolltech to link the
 *   Qt code with the OpenSSL library (see
 *   <http://doc.trolltech.com/4.4/gpl.html>). The OpenSSL library is
 *   licensed under a dual license: the OpenSSL License and the original
 *   SSLeay license.
 *
 * XiVO Client is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with XiVO Client.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QSortFilterProxyModel>

#include "conflist.h"

ConfListModel::ConfListModel(QWidget *parent)
    : QAbstractTableModel(parent)
{
    COL_TITLE[ID] = tr("Room UID");
    COL_TITLE[NUMBER] = tr("Number");
    COL_TITLE[NAME] = tr("Name");
    COL_TITLE[PIN_REQUIRED] = tr("Pin code");
    COL_TITLE[MEMBER_COUNT] = tr("Member count");
    COL_TITLE[MODERATED] = tr("Moderated");
    COL_TITLE[STARTED_SINCE] = tr("Started since");

    connect(b_engine, SIGNAL(meetmeUpdate(const QVariantMap &)),
            this, SLOT(updateRoomConfigs(const QVariantMap &)));
}

void ConfListModel::updateRoomConfigs(const QVariantMap &configs)
{
    beginResetModel();

    m_room_configs = configs;
    refreshRow2Number();

    endResetModel();
}

void ConfListModel::refreshRow2Number()
{
    m_row2number = m_room_configs.keys();
}

Qt::ItemFlags ConfListModel::flags(const QModelIndex &) const
{
    return Qt::NoItemFlags;
}

int ConfListModel::rowCount(const QModelIndex&) const
{
    return m_row2number.size();
}

int ConfListModel::columnCount(const QModelIndex&) const
{
    return NB_COL;
}

QVariant ConfListModel::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole) {
        if (role == Qt::TextAlignmentRole)
            return Qt::AlignCenter;
        return QVariant();
    }

    /* Rows here are not the same than the lines displayed by the view,
     * as there is a proxy model between the view and this model,
     * that maps the displayed lines to the stored lines, see ConfList
     * constructor
     */
    int row = index.row(), col = index.column();
    if (m_row2number.size() <= row) {
        return QVariant();
    }

    const QString &room_number = m_row2number[row];
    const QVariantMap &room_config = m_room_configs[room_number].toMap();

    switch (col) {
    case ID:
        return room_number.toInt();
    case NUMBER:
        return room_config["number"].toString();
    case NAME:
        return room_config["name"].toString();
    case PIN_REQUIRED:
        return room_config["pin_required"].toString();
    case MODERATED:
        return tr("No");  // to remove
    case MEMBER_COUNT:
        return room_config["member_count"].toString();
    case STARTED_SINCE:
        return startedSince(room_config["start_time"].toDouble());
    default:
        break;
    }

    return QVariant();
}

QString ConfListModel::startedSince(double time) const
{
    if (time == 0)
        return tr("Not started");
    else if (time == -1)
        return tr("Unknown");

    uint now = QDateTime::currentDateTime().toTime_t();
    uint started_since = now - uint(time) -b_engine->timeDeltaServerClient();

    return QDateTime::fromTime_t(started_since).toUTC().toString("hh:mm:ss");
}

QVariant ConfListModel::headerData(int section,
                                   Qt::Orientation orientation,
                                   int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        return COL_TITLE[section];
    }

    return QVariant();
}

void ConfListModel::updateConfTime()
{
    QModelIndex cellChanged1 = createIndex(0, STARTED_SINCE);
    QModelIndex cellChanged2 = createIndex(this->rowCount() - 1, STARTED_SINCE);
    emit dataChanged(cellChanged1, cellChanged2);
}

ConfListView::ConfListView(QWidget *parent)
    : QTableView(parent)
{
    setSortingEnabled(true);
    setShowGrid(0);
    verticalHeader()->hide();
    horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    horizontalHeader()->setSectionsMovable(true);
    setStyleSheet("ConfListView {"
                    "border: none;"
                    "background: transparent;"
                    "color:black;"
                  "}");

    connect(this, SIGNAL(clicked(const QModelIndex &)),
            this, SLOT(onViewClick(const QModelIndex &)));
}

void ConfListView::onViewClick(const QModelIndex &model)
{
    QString number = model.sibling(model.row(), ConfListModel::ID).data().toString();
    QString roomName = model.sibling(model.row(), ConfListModel::NAME).data().toString();
    QString roomNumber = model.sibling(model.row(), ConfListModel::NUMBER).data().toString();


    if (number != "") {
        b_engine->pasteToDial(roomNumber);
        QTimer *timer = new QTimer(this);
        timer->setSingleShot(true);
        timer->setProperty("number", number);
        connect(timer, SIGNAL(timeout()), parentWidget(), SLOT(openConfRoom()));
        timer->start(10);
    }
}


void ConfListView::contextMenuEvent(QContextMenuEvent * event)
{
    const QModelIndex &index = indexAt(event->pos());

    QString number = index.sibling(index.row(), ConfListModel::ID).data().toString();
    QString roomName = index.sibling(index.row(), ConfListModel::NAME).data().toString();
    QString roomNumber = index.sibling(index.row(), ConfListModel::NUMBER).data().toString();

    QMenu *menu = new QMenu(this);

    QAction *action = new QAction(
        tr("Get in room %1 (%2)").arg(roomName).arg(roomNumber), menu);

    action->setProperty("number", number);
    connect(action, SIGNAL(triggered(bool)),
	    parentWidget(), SLOT(openConfRoom()));
    connect(action, SIGNAL(triggered(bool)),
	    parentWidget(), SLOT(phoneConfRoom()));

    menu->addAction(action);
    menu->exec(QCursor::pos());
}


ConfList::ConfList(XletConference *parent)
    : QWidget(), m_manager(parent)
{
    QVBoxLayout *vBox = new QVBoxLayout(this);
    QHBoxLayout *hBox = new QHBoxLayout();

    // this contains the data, unordered
    m_model = new ConfListModel(this);
    m_model->setObjectName("conflist_model");

    QTimer * timer_display = new QTimer(this);
    connect(timer_display, SIGNAL(timeout()),
            m_model, SLOT(updateConfTime()));
    timer_display->start(1000);

    // this maps the indexes between the sorted view and the unordered model
    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(m_model);
    proxyModel->setDynamicSortFilter(true); /* sorts right on insertion, instead
    of half a second after the window has appeared */

    // this displays the sorted data
    ConfListView *view = new ConfListView(this);
    view->setModel(proxyModel);
    view->hideColumn(ConfListModel::ID);
    view->hideColumn(ConfListModel::MODERATED);
    view->sortByColumn(ConfListModel::NAME, Qt::AscendingOrder);

    hBox->addWidget(view);

    vBox->addLayout(hBox);
    setLayout(vBox);
}

void ConfList::phoneConfRoom()
{
    const QString &roomNumber = sender()->property("number").toString();
    QVariantMap members = m_model->getMembers(roomNumber);

    b_engine->actionDial(roomNumber);
    m_manager->openConfRoom(roomNumber, members);
}

void ConfList::openConfRoom()
{
    const QString &number = sender()->property("number").toString();
    QVariantMap members = m_model->getMembers(number);

    m_manager->openConfRoom(number, members);
}

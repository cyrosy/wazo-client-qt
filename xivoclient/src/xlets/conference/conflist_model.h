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

#ifndef __CONFLIST_MODEL_H__
#define __CONFLIST_MODEL_H__

#include <QLabel>
#include <QTimer>
#include <QVBoxLayout>
#include <QTableView>
#include <QModelIndex>
#include <QVariant>
#include <QAbstractTableModel>
#include <QHeaderView>
#include <QMenu>
#include <QAction>
#include <QMouseEvent>

#include <xletlib/functests.h>
#include <baseengine.h>


class ConfListModel : public QAbstractTableModel
{
    Q_OBJECT
    FUNCTESTED

    public:
        enum ColOrder {
            ID,
            NAME,
            NUMBER,
            PIN_REQUIRED,
            MODERATED,
            MEMBER_COUNT,
            STARTED_SINCE,
            NB_COL
        };
        ConfListModel(QWidget *parent = NULL);
        QVariantMap getMembers(const QString &number) { return m_room_configs[number].toMap()["members"].toMap(); }
    public slots:
        void updateConfTime();
        void updateRoomConfigs(const QVariantMap &);
    private:
        void refreshRow2Number();
        QString startedSince(double time) const;
        int rowCount(const QModelIndex& = QModelIndex()) const;
        int columnCount(const QModelIndex&) const;
        QVariant data(const QModelIndex&, int) const;
        QVariant headerData(int , Qt::Orientation, int) const;
        Qt::ItemFlags flags(const QModelIndex &) const;
        QStringList m_row2number;
        QVariantMap m_room_configs;
        QString COL_TITLE[NB_COL];
};

#endif

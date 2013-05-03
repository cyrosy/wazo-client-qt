/* XiVO Client
 * Copyright (C) 2013, Avencall
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

#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include <QtGui>
#include <QList>
#include <QMainWindow>

#include <systray_manager.h>
#include <login_widget/login_widget.h>
#include <config_widget/config_widget.h>
#include <ui_main_widget.h>
#include <xletlib/functests.h>


class MainWindow : public QMainWindow
{
    Q_OBJECT
    FUNCTESTED

    public:
        MainWindow(QWidget* parent = 0);
        ~MainWindow();

    private slots:
        void showMessageBox(const QString &);
        void showLogin();
        void hideLogin();
        void showConfDialog();
        void cleanConfDialog();
        void engineStopped();
        void engineStarted();
        void connectionStateChanged();

    private:
        QWidget *m_main_widget;
        ConfigWidget *m_config_widget;
        LoginWidget *m_login_widget;

        QString m_appliname;

        Ui::MainWindow ui;
};

#endif

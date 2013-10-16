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

#ifndef __CENTRAL_WIDGET_H__
#define __CENTRAL_WIDGET_H__

#include <QVBoxLayout>
#include <QFrame>

#include <ui_loading_dialog.h>

class LoginWidget;
class MainWidget;
class MainWindow;


class CentralWidget : public QFrame
{
    Q_OBJECT

    public:
        CentralWidget(MainWindow *parent);
        ~CentralWidget();

    private slots:
        void initialize();
        void initializing();
        void initialized();
        void setStatusNotLogged();
        void setStatusLogged();

    private:
        void showLoading();
        void hideLoading();
        void showLoginWidget();
        void showMainWidget();

        QVBoxLayout *m_layout;
        MainWindow *m_main_window;
        LoginWidget *m_login_widget;
        MainWidget *m_main_widget;

        Ui::loading_dialog *ui_loading_dialog;
        QDialog *m_loading_dialog;
};

#endif

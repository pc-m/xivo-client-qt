/* XiVO Client
 * Copyright (C) 2007-2013, Avencall
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

#ifndef __IDENTITYVOICEMAIL_H__
#define __IDENTITYVOICEMAIL_H__

#include <QWidget>
#include <QVariant>
#include <QPushButton>
#include <QLabel>

class QGridLayout;
class UserInfo;
class VoiceMailInfo;

/*! \brief Identity Voice mail display
 */
class IdentityVoiceMail : public QWidget
{
    Q_OBJECT

    public:
        IdentityVoiceMail(QWidget *parent);
        void setVoiceMailId(const QString &);
        void svcSummary(QVariantMap &svcstatus, const UserInfo *ui);
    public slots:
        void updateVoiceMailConfig(const QString &);
        void updateVoiceMailStatus(const QString &);
    protected:
        void queryVM();
    private slots:
        void callVoiceMail();
        void updateMessageIndicators(const int nbOfNewMessages, const int nbOfReadMessages);


    private:
        QString m_xvoicemailid;
        bool m_initialized;
        const VoiceMailInfo * m_voicemailinfo;

        QGridLayout * m_layout;       //!< layout
        QPushButton * m_iconButton;   //!< icon
        QPushButton * newMessageIndicator;
        QPushButton * oldMessageIndicator;
        QLabel * m_name;              //!< box name
        QPixmap icon_no_message;
        QPixmap icon_new_message;
};

#endif

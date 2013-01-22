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

#ifndef __PHONEINFO_H__
#define __PHONEINFO_H__

#include <QString>
#include <QStringList>
#include <QVariant>
#include <QVariantMap>

#include "xinfo.h"
#include "baselib_export.h"

class BASELIB_EXPORT PhoneInfo : public XInfo
{
    public:
        PhoneInfo(const QString &, const QString &);
        bool updateConfig(const QVariantMap &);
        bool updateStatus(const QVariantMap &);
        const QString & context() const { return m_context; };
        const QString & number() const { return m_number; };
        const QString & identity() const { return m_identity; };
        const QString & protocol() const { return m_protocol; };
        const QString & iduserfeatures() const { return m_iduserfeatures; };
        QString xid_user_features() const;
        int rules_order() const { return m_rules_order; };
        int simultcalls() const { return m_simultcalls; };

        const QStringList & channels() const { return m_channels; };
        const QStringList & xchannels() const { return m_xchannels; };
        const QString & hintstatus() const { return m_hintstatus; };

        bool enableunc() const { return m_enableunc; };
    private:
        QString m_protocol;
        QString m_context;
        QString m_number;
        QString m_identity;
        QString m_iduserfeatures;
        int m_rules_order;
        int m_simultcalls;

        bool m_initialized;
        bool m_enable_hint;
        QString m_hintstatus;
        QStringList m_channels;
        QStringList m_xchannels;

        bool m_enablerna;
        bool m_enableunc;
        bool m_enablebusy;
        QString m_destrna;
        QString m_destunc;
        QString m_destbusy;

        bool m_enableautomon;
        bool m_enablednd;
        bool m_enablevoicemail;
        bool m_enablexfer;
        bool m_incallfilter;

    public:
};

namespace PhoneHint {
    static QString available = "0";
    static QString unavailable = "4";
};

namespace phone {

    BASELIB_EXPORT const PhoneInfo * findByIdentity(const QString & identity);

}

#endif

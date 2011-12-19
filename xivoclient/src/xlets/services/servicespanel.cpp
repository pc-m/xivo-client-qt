/* XiVO Client
 * Copyright (C) 2007-2011, Avencall
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

#include <QGroupBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>

#include <baseengine.h>

#include "servicespanel.h"

const QStringList chkcapas = (QStringList() << "enablevoicemail" << "incallfilter" << "enablednd");
const QStringList fwdcapas = (QStringList() << "fwdrna" << "fwdbusy" << "fwdunc");

Q_EXPORT_PLUGIN2(xletservicesplugin, XLetServicesPlugin);

XLet* XLetServicesPlugin::newXLetInstance(QWidget *parent)
{
    b_engine->registerTranslation(":/obj/services_%1");
    return new ServicesPanel(parent);
}

ServicesPanel::ServicesPanel(QWidget * parent)
    : XLet(parent)
{
    setTitle(tr("Services"));
    m_capalegend["enablevoicemail"] = tr("Voice &Mail");
    m_capalegend["incallfilter"]    = tr("Call &Filtering");
    m_capalegend["enablednd"]       = tr("Do Not &Disturb");
    m_capalegend["fwdrna"]          = tr("Forward on &No Answer");
    m_capalegend["fwdbusy"]         = tr("Forward on &Busy");
    m_capalegend["fwdunc"]          = tr("&Unconditional Forward");

    m_capas = b_engine->getConfig("services").toStringList();

    int line = 0;

    QGroupBox *groupBox1 = new QGroupBox(tr("Services"), this);
    groupBox1->setAlignment( Qt::AlignLeft );
    groupBox1->hide();
    QGridLayout *gridlayout1 = new QGridLayout(groupBox1);

    foreach (QString capa, chkcapas) {
        if (m_capas.contains(capa)) {
            m_chkopt[capa] = new WaitingWidget<QCheckBox>(
                new QCheckBox(m_capalegend[capa], this));
            m_chkopt[capa]->widget()->setObjectName("service");
            m_chkopt[capa]->widget()->setProperty("capa", capa);
            gridlayout1->addWidget(m_chkopt[capa], line++, 0, 1, 0);
        }
    }

    QGroupBox *groupBox2 = new QGroupBox(tr("Call Forwards"), this);
    groupBox2->setAlignment(Qt::AlignLeft);
    groupBox2->hide();
    QGridLayout *gridlayout2 = new QGridLayout(groupBox2);
    QHash<QString, QLabel *> label;

    foreach (QString capa, fwdcapas) {
        if (m_capas.contains(capa)) {
            m_forward[capa] = new WaitingWidget<QCheckBox>(
                 new QCheckBox(m_capalegend[capa], this));
            m_forward[capa]->widget()->setObjectName("service");
            m_forward[capa]->widget()->setProperty("capa", capa);
            gridlayout2->addWidget(m_forward[capa], line++, 0, 1, 0);

            label[capa] = new QLabel(tr("Destination"), this);
            gridlayout2->addWidget(label[capa], line, 0);
            m_forwarddest[capa] = new WaitingWidget<QLineEdit>(
                 new QLineEdit(this));
            m_forwarddest[capa]->widget()->setProperty("capa", capa);
            gridlayout2->addWidget(m_forwarddest[capa], line++, 1);
            label[capa]->setObjectName("service");
        }
    }

    QVBoxLayout *vlayout = new QVBoxLayout(this);
    if (m_capas.contains("enablevoicemail") ||
        m_capas.contains("incallfilter") ||
        m_capas.contains("enablednd")) {
        groupBox1->show();
        vlayout->addWidget(groupBox1);
    }
    if (m_capas.contains("fwdrna") ||
        m_capas.contains("fwdbusy") ||
        m_capas.contains("fwdunc")) {
        groupBox2->show();
        vlayout->addWidget(groupBox2);
    }
    vlayout->addStretch(1);

    resetWidgets();

    // connect signals/slots

    foreach (QString capa, chkcapas) {
        if (m_capas.contains(capa)) {
            connect(m_chkopt[capa]->widget(), SIGNAL(clicked(bool)),
                    this, SLOT(chkoptToggled(bool)));
        }
    }
    foreach (QString capa, fwdcapas) {
        if (m_capas.contains(capa)) {
            connect(m_forward[capa]->widget(), SIGNAL(clicked(bool)),
                    this, SLOT(forwardToggled(bool)));
            connect(m_forwarddest[capa]->widget(), SIGNAL(lostFocus()),
                    this, SLOT(forwardLostFocus()));
        }
    }

    connect(b_engine, SIGNAL(resetServices()),
            this, SLOT(resetWidgets()));

    connect(b_engine, SIGNAL(optChanged(const QString &)),
            this, SLOT(syncOpt(const QString &)));
    connect(b_engine, SIGNAL(forwardUpdated(const QString &)),
            this, SLOT(syncForward(const QString &)) );

    connect(b_engine, SIGNAL(servicePutIsOK(const QString &, const QString &)),
            this, SLOT(servicePutIsOK(const QString &, const QString &)));

    connect(b_engine, SIGNAL(updateUserConfig(const QString &, const QVariantMap &)),
            this, SLOT(updateUserConfig(const QString &, const QVariantMap &)));
    connect(b_engine, SIGNAL(updatePhoneConfig(const QString &)),
            this, SLOT(updatePhoneConfig(const QString &)));

    b_engine->askServices();
}

void ServicesPanel::updateUserConfig(const QString & xuserid, const QVariantMap & datamap)
{
    if (xuserid == m_xuserid) {
        foreach (QVariant config_var, datamap["config"].toMap().keys()) {
            QString config_str = config_var.toString();
            if (chkcapas.contains(config_str)) {
                syncOpt(config_str);
            } else {
                // this will filter the configstr to retain only enable* and dest*
                syncForward(config_str);
            }
        }
    }
}

void ServicesPanel::updatePhoneConfig(const QString &)
{
}

void ServicesPanel::resetWidgets()
{
    //qDebug() << Q_FUNC_INFO;
    foreach (QString capa, chkcapas) {
        if (m_capas.contains(capa))
            m_chkopt[capa]->widget()->setChecked(false);
    }
    foreach (QString capa, fwdcapas) {
        if (m_capas.contains(capa)) {
            m_forward[capa]->widget()->setChecked(false);
            m_forwarddest[capa]->widget()->setText("");
        }
    }
}

void ServicesPanel::chkoptToggled(bool b)
{
    QString capa = sender()->property("capa").toString();
    m_chkopt[capa]->lock();
    b_engine->servicePutOpt(capa, b);
}

void ServicesPanel::forwardToggled(bool b)
{
    QString capa  = sender()->property("capa").toString();
    QString fdest = m_forwarddest[capa]->widget()->text();

    if(! (b && fdest.isEmpty())) {
        m_forward[capa]->lock();
        m_forwarddest[capa]->lock();
        m_replyids[capa] = b_engine->servicePutForward(capa, b, fdest);
    } else {
        qDebug() << "Forwarding calls to nothing ... This should not happen !";
    }
}

void ServicesPanel::forwardLostFocus()
{
    QString capa = sender()->property("capa").toString();
    QString fdest = m_forwarddest[capa]->widget()->text();
    //qDebug() << Q_FUNC_INFO << capa;

    if (fdest.isEmpty()) {
        m_forward[capa]->widget()->setChecked(false);
        m_forward[capa]->widget()->setEnabled(false);
    }
    m_forward[capa]->lock();
    m_forwarddest[capa]->lock();
    m_replyids[capa] = b_engine->servicePutForward(capa,
                                                   m_forward[capa]->widget()->isChecked(),
                                                   fdest);
}

/*!
 * The main reason of this function is to catch reply of the CTI server in case
 * no changes have been sent, for example when entering and exiting focus of a
 * text field without changing it. It prevents the WaitingWidgets to wait for an
 * update that will never come, because there is no new data, then nothing to
 * update.
 */
void ServicesPanel::servicePutIsOK(const QString & replyid, const QString & warning_string)
{
    if (warning_string == "no changes") {
        foreach(QString capa, m_replyids.keys()) {
            if (m_replyids[capa] == replyid) {
                m_forward[capa]->unlock(! m_forwarddest[capa]->widget()->text().isEmpty());
                m_forwarddest[capa]->unlock();
            }
        }
    }
}

// The following actions are entered in when the status is received from the server (init or update)
/*! \brief sync widgets with userinfo
 */
void ServicesPanel::syncOpt(const QString & capa)
{
    if (m_ui == NULL)
        return;

    if (m_capas.contains(capa)) {
        if (capa == "enablednd")
            m_chkopt[capa]->widget()->setChecked(m_ui->enablednd());
        if (capa == "incallfilter")
            m_chkopt[capa]->widget()->setChecked(m_ui->incallfilter());
        if (capa == "enablevoicemail")
            m_chkopt[capa]->widget()->setChecked(m_ui->enablevoicemail());
        m_chkopt[capa]->unlock();
    }
}

/*! \brief sync widgets with userinfo
 */
void ServicesPanel::syncForward(const QString & capa)
{
    if (m_ui == NULL)
        return;
    if (capa.startsWith("enable")) {
        QString thiscapa = "fwd" + capa.mid(6);
        if (m_capas.contains(thiscapa)) {
            if (capa == "enablebusy") {
                m_forward[thiscapa]->widget()->setChecked(m_ui->enablebusy());
            } else if (capa == "enablerna") {
                m_forward[thiscapa]->widget()->setChecked(m_ui->enablerna());
            } else if (capa == "enableunc") {
                m_forward[thiscapa]->widget()->setChecked(m_ui->enableunc());
            }
            m_forward[thiscapa]->unlock(! m_forwarddest[thiscapa]->widget()->text().isEmpty());
            m_forwarddest[thiscapa]->unlock();
        }
    } else if (capa.startsWith("dest")) {
        QString thiscapa = "fwd" + capa.mid(4);
        if (m_capas.contains(thiscapa)) {
            if (capa == "destbusy") {
                m_forwarddest[thiscapa]->widget()->setText(m_ui->destbusy());
            } else if (capa == "destrna") {
                m_forwarddest[thiscapa]->widget()->setText(m_ui->destrna());
            } else if (capa == "destunc") {
                m_forwarddest[thiscapa]->widget()->setText(m_ui->destunc());
            }
            m_forward[thiscapa]->unlock(! m_forwarddest[thiscapa]->widget()->text().isEmpty());
            m_forwarddest[thiscapa]->unlock();
        }
    }
}

#include <QDebug>

#include "aastrasipnotify.h"
#include "phoneinfo.h"
#include "userinfo.h"
#include "popcaastra.h"
#include "transferedwidget.h"

class QHBoxLayout;

TransferedWidget::TransferedWidget(QString number, const QString & tname,
        const QString & tnum, QWidget * parent):
    QWidget(parent),
    m_number(number)
{
    //qDebug() << Q_FUNC_INFO;

    m_time_transfer = b_engine->timeServer();
    m_readyToBeRemoved = false;

    m_layout = new QHBoxLayout(this);
    m_lbl_status = new QLabel(this);
    m_layout->addWidget(m_lbl_status);
    m_lbl_transfered_name = new QLabel(this);
    m_layout->addWidget(m_lbl_transfered_name);
    m_lbl_transfered_number = new QLabel(this);
    m_layout->addWidget(m_lbl_transfered_number);
    m_lbl_called_name = new QLabel(this);
    m_layout->addWidget(m_lbl_called_name);
    m_lbl_called_number = new QLabel(this);
    m_layout->addWidget(m_lbl_called_number);
    m_lbl_time = new QLabel(this);
    m_layout->addWidget(m_lbl_time);

    m_lbl_transfered_name->setText(tname);
    m_lbl_transfered_number->setText(tnum);

    m_interceptAction = new QAction(tr("&Intercepet"), this);
    m_interceptAction->setStatusTip(tr("Intercept a transfered call"));
    connect(m_interceptAction, SIGNAL(triggered()),
            this, SLOT(doIntercept()));

    setXphoneId();
    updateWidget();
}

/*! \brief returns the exten number that this called is being transfered to */
const QString & TransferedWidget::number() const
{
    return m_number;
}

/*! \brief refresh widgets */
void TransferedWidget::updateWidget()
{
    //qDebug() << Q_FUNC_INFO;

    const PhoneInfo * called = b_engine->phone(m_called_phone_id);
    if (called == NULL) return;

    if (b_engine->getOptionsPhoneStatus().contains(called->hintstatus())) {
        if (called->hintstatus() == "0" && !(m_hintstatus.isEmpty() || m_hintstatus == "0")) {
            m_readyToBeRemoved = true;
        }
        m_hintstatus = called->hintstatus();
        QVariantMap s = b_engine->getOptionsPhoneStatus().value(m_hintstatus).toMap();
        QString string = s.value("longname").toString();
        m_lbl_status->setText(
            b_engine->getOptionsPhoneStatus().value(m_hintstatus).toMap()
                .value("longname").toString());
    } else {
        m_lbl_status->setText(tr("Unknown"));
    }

    m_lbl_called_name->setText(tr("Unknown"));
    foreach (QString user, b_engine->iterover("users").keys()) {
        QStringList phonelist = b_engine->user(user)->phonelist();
        foreach (QString p, phonelist) {
            if (b_engine->phone(p)->number() == m_number) {
                m_lbl_called_name->setText(b_engine->user(user)->fullname());
            }
        }
    }
    m_lbl_called_number->setText(m_number);
    m_lbl_time->setText(b_engine->timeElapsed(m_time_transfer));
}

/*! \brief set m_called_phone_id */
void TransferedWidget::setXphoneId()
{
    //qDebug() << Q_FUNC_INFO;
    foreach (QString xphoneid, b_engine->iterover("phones").keys()) {
        const PhoneInfo * phone = b_engine->phone(xphoneid);
        if (phone->number() == m_number) {
            m_called_phone_id = xphoneid;
            return;
        }
    }
}

/*! \brief check if the widget is ready to be removed */
bool TransferedWidget::readyToBeRemoved() const
{
    //qDebug() << Q_FUNC_INFO;
    return m_readyToBeRemoved;
}

/*! \brief cancel a blind transfer and retrieves the call */
void TransferedWidget::doIntercept()
{
    //qDebug() << Q_FUNC_INFO;
    emit intercept(m_number);
}

void TransferedWidget::contextMenuEvent(QContextMenuEvent *event)
{
    //qDebug() << Q_FUNC_INFO;
    QMenu contextMenu;
    contextMenu.addAction(m_interceptAction);
    contextMenu.exec(event->globalPos());
}

TransferedWidget::~TransferedWidget()
{
    // qDebug() << Q_FUNC_INFO;
}

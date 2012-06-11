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

#include "cti_server.h"

CTIServer::CTIServer(QSslSocket * socket)
    : QObject(NULL), m_socket(socket)
{
    connect(socket, SIGNAL(disconnected()),
            this, SLOT(ctiSocketDisconnected()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(ctiSocketError(QAbstractSocket::SocketError)));
}

void CTIServer::ctiSocketError(QAbstractSocket::SocketError socketError)
{
    qDebug() << Q_FUNC_INFO << socketError;
    switch (socketError) {
        // ~ once connected
        case QAbstractSocket::RemoteHostClosedError:
            ctiSocketClosedByRemote();
            break;

        // ~ when trying to connect
        case QAbstractSocket::ConnectionRefusedError:
            emit failedToConnect("socket_error_connectionrefused");
            break;
        case QAbstractSocket::HostNotFoundError:
            emit failedToConnect("socket_error_hostnotfound");
            break;
        case QAbstractSocket::NetworkError:
            emit failedToConnect("socket_error_network");
            break;
        case QAbstractSocket::SocketTimeoutError:
            emit failedToConnect("socket_error_timeout");
            break;
        case QAbstractSocket::SslHandshakeFailedError:
            emit failedToConnect("socket_error_sslhandshake");
            break;
        case QAbstractSocket::UnknownSocketError:
            emit failedToConnect("socket_error_unknown");
            break;

        default:
            // see http://doc.trolltech.com/4.6/qabstractsocket.html#SocketError-enum
            // for unmanaged error cases
            failedToConnect(QString("socket_error_unmanagedyet:%1").arg(socketError));
            break;
    }
}

void CTIServer::ctiSocketClosedByRemote()
{
    qDebug() << Q_FUNC_INFO;
    b_engine->emitMessage(tr("Connection lost with XiVO CTI server"));
    b_engine->startTryAgainTimer();
    emit failedToConnect("socket_error_remotehostclosed");

    QTimer * timer = new QTimer(this);
    timer->setProperty("stopper", "connection_lost");
    timer->setSingleShot(true);
    connect(timer, SIGNAL(timeout()),
            b_engine, SLOT(stop()));
    timer->start();
}

/*! \brief called when the socket is closed, whatever reason
 */
void CTIServer::ctiSocketDisconnected()
{
    qDebug() << Q_FUNC_INFO;
}

void CTIServer::connectToServer(ConnectionConfig config)
{
    this->connectSocket(config.main_address,
                        config.main_port,
                        config.main_encrypt);
}

void CTIServer::connectSocket(const QString & address,
                              unsigned port,
                              bool encrypted)
{
    m_socket->abort();
    if (encrypted) {
        m_socket->connectToHostEncrypted(address, port);
    } else {
        m_socket->connectToHost(address, port);
    }
}

bool CTIServer::connected()
{
    return m_socket->state() == QAbstractSocket::ConnectedState;
}

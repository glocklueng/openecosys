/**
     Copyright (C) 2011 IntRoLab
     http://introlab.gel.usherbrooke.ca
     Dominic Letourneau, ing. M.Sc.A.
     Dominic.Letourneau@USherbrooke.ca

     This file is part of OpenECoSys/NetworkViewer.
     OpenECoSys/NetworkViewer is free software: you can redistribute it and/or modify
     it under the terms of the GNU General Public License as published by the Free Software
     Foundation, either version 3 of the License, or (at your option) any later version.
     OpenECoSys/NetworkViewer is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
     or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
     You should have received a copy of the GNU General Public License along with
     OpenECoSys/NetworkViewer. If not, see http://www.gnu.org/licenses/.

 */

#include "NETVRemoteClient.h"
#include <QDebug>
#include "NETVDevice.h"
#include "NETVMessageEvent.h"
#include <QCoreApplication>


NETVRemoteClient::NETVRemoteClient(const QString &hostname, int port, QObject *parent)
    :   QTcpSocket(parent)
{
    //Let's try to connect
    connect(this,SIGNAL(connected()),this,SLOT(clientConnected()));
    connect(this,SIGNAL(disconnected()),this,SLOT(clientDisconnected()));
    connect(this,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(writeError(QAbstractSocket::SocketError)));

    //Connect to server
    connectToHost(hostname,port);

}


void NETVRemoteClient::clientConnected(void)
{
    qDebug("NETVRemoteClient::clientConnected(void)");

    //We are now connected, connecting readyRead signal
    connect(this,SIGNAL(readyRead()),this,SLOT(readyReadSocket()));
}

void NETVRemoteClient::clientDisconnected(void)
{

}

void NETVRemoteClient::readyReadSocket(void)
{
    //qDebug("NETVRemoteClient::readyReadSocket(void) bytesAvailable : %li",bytesAvailable());

    //Read all data on socket
    while(bytesAvailable() > 0)
    {
        //Read one message
        NETV_MESSAGE msg;
        if (msg.unserialize(*this))
        {
            //qDebug("NETVRemoteClient::readyReadSocket(void) - parsing OK");
            emit messageReady(msg);
        }
        else
        {
            qDebug("NETVRemoteClient::readyReadSocket(void) - Parsing error!");
            break;
        }

    }
}

void NETVRemoteClient::writeError(QAbstractSocket::SocketError socketError)
{
    qDebug("NETVRemoteClient::writeError(QAbstractSocket::SocketError socketError = %i)",(int) socketError);

    QString m_error;

    switch (socketError)
    {
        case QAbstractSocket::RemoteHostClosedError:
            m_error = tr("TCPC_REMOTE_HOST_CLOSED");
            break;
        case QAbstractSocket::HostNotFoundError:
            m_error = tr("TCPC_HOST_NOT_FOUND");
            break;
        case QAbstractSocket::ConnectionRefusedError:
            m_error = tr("TCPC_CONNECTION_REFUSED");
            break;
        case QAbstractSocket::SocketTimeoutError:
            m_error = tr("TCPC_TIMEOUT");
            break;
        case QAbstractSocket::NetworkError:
            m_error = tr("TCPC_NETWORK_TIMEOUT");
            break;
        default:
            m_error = tr("TCPC_UNKNOWN_ERROR");
            break;
    }

    qDebug() << m_error;

}

bool NETVRemoteClient::event( QEvent * e )
{
    if (e->type() == QEvent::User)
    {
        //qDebug("NETVRemoteClient::event %p QEvent::User",e);
        if (NETVMessageEvent *event = dynamic_cast<NETVMessageEvent*>(e))
        {
            //we need to send the data
            if (isWritable())
            {
                event->getMessage().serialize(*this);

                //Flush socket
                flush();
            }

            return true;
        }
    }

    return QTcpSocket::event(e);
}


void NETVRemoteClient::sendMessage(const NETV_MESSAGE &msg)
{
    QCoreApplication::postEvent(this,new NETVMessageEvent(msg),Qt::HighEventPriority);
}

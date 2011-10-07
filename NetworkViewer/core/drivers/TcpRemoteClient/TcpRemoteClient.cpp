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

#include "TcpRemoteClient.h"
#include <QDebug>
#include <QCoreApplication>
#include "NETVMessageEvent.h"

static bool TCP_REMOTE_CLIENT_DEVICE_INIT = NETVDevice::registerDeviceFactory("TcpRemoteClient",new NETVDevice::DeviceFactory<TcpRemoteClient>("localhost;12345","Host;port"));


TcpRemoteClient::TcpRemoteClient(const char *args)
    :   m_remoteClient(NULL), m_mutex(QMutex::Recursive)
{
    if (args)
    {
        initialize(args);
    }
}

NETVDevice::State TcpRemoteClient::initialize(const char* args)
{

    //qDebug("NETVDevice::State TcpRemoteClient::initialize %s",args);

    QStringList argsList = QString(args).split(";");

    if (argsList.size() >= 2)
    {
        QString host = argsList[0];
        int port = argsList[1].toInt();

        m_remoteClient = new NETVRemoteClient(host,port,this);

        //Can this work?
        connect(m_remoteClient,SIGNAL(messageReady(NETV_MESSAGE)),this,SLOT(messageReady(NETV_MESSAGE)));

        return NETVDevice::NETVDEVICE_OK;
    }
    else
    {
        return NETVDevice::NETVDEVICE_FAIL;
    }
}

NETVDevice::State TcpRemoteClient::sendMessage(NETV_MESSAGE &message)
{
    if (m_remoteClient)
    {
        //qDebug("m_remoteClientState : %i",m_remoteClient->state());

        if (m_remoteClient->state() == QAbstractSocket::ConnectedState)
        {

            //We are not yet ready to do this...
            //QCoreApplication::postEvent(m_remoteClient,new NETVMessageEvent(message));
            return NETVDevice::NETVDEVICE_OK;
        }
        else
        {
            return NETVDevice::NETVDEVICE_FAIL;
        }

    }

    return NETVDevice::NETVDEVICE_NOT_INITIALIZED;
}


NETVDevice::State TcpRemoteClient::recvMessage(NETV_MESSAGE &message)
{
    if (m_remoteClient)
    {
        QMutexLocker lock(&m_mutex);

        if (m_msgList.size() > 0)
        {
            message = m_msgList.front();
            m_msgList.pop_front();
            return NETVDevice::NETVDEVICE_OK;
        }
        else
        {
            return NETVDevice::NETVDEVICE_UNDERFLOW;
        }
    }

    return NETVDevice::NETVDEVICE_FAIL;
}


bool TcpRemoteClient::newMessageReady()
{
    QMutexLocker lock(&m_mutex);
    return (!m_msgList.empty());
}


bool TcpRemoteClient::event(QEvent *event)
{
    return QObject::event(event);
}

void TcpRemoteClient::messageReady(const NETV_MESSAGE &msg)
{
    //qDebug("void TcpRemoteClient::messageReady(const NETV_MESSAGE &msg)");
    QMutexLocker lock(&m_mutex);
    m_msgList.push_back(msg);
}

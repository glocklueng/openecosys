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

static bool TCP_REMOTE_CLIENT_DEVICE_INIT = NETVDevice::registerDeviceFactory("TcpRemoteClient",new NETVDevice::DeviceFactory<TcpRemoteClient>("localhost;12345","Host;port"));


TcpRemoteClient::TcpRemoteClient(const char *args)
    :   m_remoteClient(NULL)
{
    if (args)
    {
        initialize(args);
    }
}

NETVDevice::State TcpRemoteClient::initialize(const char* args)
{
    QStringList argsList = QString(args).split(";");

    if (argsList.size() >= 2)
    {
        QString host = argsList[0];
        int port = argsList[1].toInt();

        m_remoteClient = new NETVRemoteClient(host,port,this);

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
        return NETVDevice::NETVDEVICE_OK;
    }

    return NETVDevice::NETVDEVICE_FAIL;
}


NETVDevice::State TcpRemoteClient::recvMessage(NETV_MESSAGE &message)
{
    if (m_remoteClient)
    {
        return NETVDevice::NETVDEVICE_UNDERFLOW;
    }

    return NETVDevice::NETVDEVICE_FAIL;
}


bool TcpRemoteClient::newMessageReady()
{
    return false;
}


bool TcpRemoteClient::event(QEvent *event)
{

}

/**
     Copyright (C) 2009-2012 IntRoLab
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

#include "NETVBroadcaster.h"
#include <QtDebug>
#include <QBuffer>
#include <QNetworkInterface>



NETVBroadcaster::NETVBroadcaster(int udp_port, int tcp_port, QObject *parent)
    :   QObject(parent), m_udpPort(udp_port), m_tcpPort(tcp_port)
{

    qDebug("NETVBroadcaster starting on port: %i, tcp: %i", m_udpPort, m_tcpPort);

    //Create socket
    m_socket = new QUdpSocket(this);
    m_socket->bind(m_udpPort,QUdpSocket::DefaultForPlatform);
    m_timerID = startTimer(1000);//1000 ms
}

void NETVBroadcaster::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_timerID)
    {
        //We should broadcast...
        QBuffer buffer;
        buffer.open(QIODevice::WriteOnly);
        m_info.serialize(buffer);

        //Broadcast information (to all interfaces)
        //qDebug("Broadcasting size: %i",buffer.data().size());
        //qDebug() << buffer.data();

        QList<QNetworkInterface> interfaceList =  QNetworkInterface::allInterfaces();

        for (unsigned int i = 0; i < interfaceList.size(); i++)
        {
            if ((interfaceList[i].flags() & QNetworkInterface::CanBroadcast) &&
                    (interfaceList[i].flags() & QNetworkInterface::IsUp))
            {
                //qDebug() << "Sending on " << interfaceList[i].humanReadableName();
                QList<QNetworkAddressEntry> entries = interfaceList[i].addressEntries();
                for (unsigned int j = 0; j < entries.size(); j++)
                {
                    m_socket->writeDatagram(buffer.data(),entries[j].broadcast(), m_udpPort);
                }
            }
        }

    }
}



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

#ifndef _NETVBROADCASTER_H_
#define _NETVBROADCASTER_H_

#include <QUdpSocket>
#include <QTimer>
#include <QTimerEvent>
#include <QIODevice>
#include <QDomDocument>
#include <QDomElement>
#include <QTextStream>

class NETVRemoteInfo : public QObject
{
    Q_OBJECT;

public:

    NETVRemoteInfo(QObject *parent=NULL)
        : QObject(parent), m_name("unnamed"), m_description("no description"), m_port(-1)
    {

    }


    bool serialize(QIODevice &dev)
    {

        if (!dev.isWritable())
        {
            qDebug("NETVRemoteInfo::serialize - Device is not writeable");
            return false;
        }

        QDomDocument doc("NETVRemoteInfo");
        QDomElement generalElement = doc.createElement("general-info");

        //Name
        generalElement.setAttribute("name",m_name);
        //Description
        generalElement.setAttribute("description",m_description);
        //Port
        generalElement.setAttribute("port",QString::number(m_port));

        doc.appendChild(generalElement);


        QTextStream stream(&dev);
        doc.save(stream, 4);
        return true;
    }

    bool unserialize(QIODevice &dev)
    {
        return false;
    }


    QString m_name;
    QString m_description;
    int m_port;
};



class NETVBroadcaster : public QObject
{
    Q_OBJECT;

public:

    NETVBroadcaster(int udp_port=8888, int tcp_port=12345, QObject *parent=NULL);


protected slots:

   virtual void timerEvent(QTimerEvent *event);


protected:
    int m_timerID;
    int m_tcpPort;
    int m_udpPort;
    QUdpSocket *m_socket;
    NETVRemoteInfo m_info;

};


#endif

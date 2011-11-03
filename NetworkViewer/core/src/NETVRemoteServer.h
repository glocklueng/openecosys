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

#ifndef _NETV_REMOTE_SERVER_H_
#define _NETV_REMOTE_SERVER_H_

#include <QTcpServer>
#include "NETVDevice.h"
#include <QTcpSocket>
#include <QList>
#include "NETVInterfaceManager.h"
#include "NETVInterfaceHandler.h"
#include <QEvent>

class NETVRemoteServer : public QTcpServer, public NETVMessageObserverIF
{
    Q_OBJECT;

public:

    ///Default port for tcp communication
    static const int DEFAULT_PORT = 12345;

    NETVRemoteServer(QObject *parent=NULL);

public slots:

    void addInterface(NETVInterfaceManager *manager);
    void removeInterface(NETVInterfaceManager *manager);

protected slots:

void readyReadSocket(void);


protected:

    virtual void incomingConnection(int socketDescriptor);

    ///notify function for new messages
    virtual void notifyNETVMessage(const NETV_MESSAGE &msg);

    ///Event handling
    virtual bool event (QEvent * e);

    QList<QTcpSocket*> m_socketList;

    QList<NETVInterfaceManager*> m_managerList;
};

#endif

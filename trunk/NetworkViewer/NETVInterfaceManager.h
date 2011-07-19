/**
     Copyright (C) 2009-2011 IntRoLab
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

#ifndef _NETVINTERFACEMANAGER_H_
#define _NETVINTERFACEMANAGER_H_

#include <QObject>
#include "NETVInterfaceHandler.h"
#include "NetworkModule.h"
#include "NetworkScheduler.h"
#include "NETV_define.h"
#include <QList>

/**
    Handling of scheduling and I/O from one interface.
    \date July 15 2011
    \author Dominic
*/
class NETVInterfaceManager : public QObject, public NETVMessageObserverIF
{
    Q_OBJECT;

public:

    ///Constructor
    NETVInterfaceManager(NETVDevice *device, const char* args, QObject *parent = NULL);

    virtual ~NETVInterfaceManager();

    /**
        Message notification, will be called from recvThread of \ref NETVInterfaceHandler
        \param msg the NETV message
    */
    virtual void notifyNETVMessage(const NETV_MESSAGE &msg);

    void sendAliveRequest();

    void requestVariable(ModuleVariable *variable);

    bool addModule(NetworkModule *module);

    bool removeModule(NetworkModule *module);

    NetworkModule *getModule(unsigned int deviceID);

    NETVInterfaceHandler* getInterfaceHandler();

    QList<NetworkModule*> getModules();



signals:

    void moduleAdded(NetworkModule *module);
    void moduleRemoved(NetworkModule *module);
    void moduleActive(NetworkModule *module, bool active);

protected slots:

    void writeVariable(ModuleVariable *variable);

    void watchdogTimeout();


protected:

    /**
        Process a NETV message
        \param msg The message to process
    */
    void processCANMessage(const NETV_MESSAGE &msg);

    ///QObject event handler overload
    virtual bool event (QEvent * e);

    ///This will handle read/write on the interface
    NETVInterfaceHandler *m_handler;

    ///This will schedule variables on the interface
    NetworkScheduler *m_scheduler;

    ///All modules on this interface
    QList<NetworkModule*> m_modules;

    ///Watchdog timer
    QTimer *m_watchdogTimer;

};

#endif

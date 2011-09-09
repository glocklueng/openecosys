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

#ifndef _DSPICBOOTLOADER_H_
#define _DSPICBOOTLOADER_H_

#include "BasePlugin.h"
#include "ui_dsPICBootloader.h"
#include "NETV_define.h"
#include "NETVDevice.h"
#include "hexutils.h"
#include "NetworkModule.h"
#include "NETVInterfaceHandler.h"
#include <QTimer>
#include <QTime>
#include "hexutils.h"


#define BOOT_LOADER_ADDRESS 0x9000
#define BOOT_LOADER_PAGE_SIZE 64
#define BOOT_LOADER_WORD_SIZE 2


class dsPICBootloader : public BasePlugin, public NETVMessageObserverIF
{
    Q_OBJECT;


public:


    dsPICBootloader(NetworkView *view);

    virtual ~dsPICBootloader();

    ///init not used right now
    virtual void init();

    ///terminate not used right now
    virtual void terminate();


    /**
        Message notification, will be called from recvThread of \ref NETVInterfaceHandler
        \param msg the NETV message
    */
    virtual void notifyNETVMessage(const NETV_MESSAGE &msg);

protected slots:

    void loadHEX();
    void upload();
    void timeout();
    void stop();

    void printMessage(const QString &message);

    void addEmergencyProgram(unsigned int moduleID);
    void addResetCommand(unsigned int moduleID);
    void addSetBaseAddress(unsigned int moduleID, unsigned int address);
    void addSendDataInc(unsigned int moduleID, QVector<unsigned char> &data);

protected:


    NETVInterfaceHandler *m_interface;
    Ui::dsPICBootloader m_ui;
    QList<NETV_MESSAGE> m_msgQueue;


    void generateMessageQueue(hexutils::hex32doc &doc, NetworkModule *module);
    void generateMemoryMap(hexutils::hex32doc &doc);

    ///QObject event handler overload
    virtual bool event (QEvent * e);

    QTimer *m_timer;
    QTime m_elapsed;
    QVector<unsigned char> m_memoryData;
    hexutils::hex32doc m_doc;
    NetworkModule *m_module;

};

#endif

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



//Bootloader commands
enum
{
    BOOTLOADER_SET_BASE_ADDR,
    BOOTLOADER_GET_BASE_ADDR,
    BOOTLOADER_SET_MODULE_ADDR,
    BOOTLOADER_GET_MODULE_ADDR,
    BOOTLOADER_SET_STATE,
    BOOTLOADER_GET_STATE,
    BOOTLOADER_SET_DELAY,
    BOOTLOADER_GET_DELAY,
    BOOTLOADER_READ_INC,
    BOOTLOADER_WRITE_INC,
    BOOTLOADER_READ_PAGE,
    BOOTLOADER_WRITE_PAGE,
    BOOTLOADER_WRITE_BOOTCONFIG,
    BOOTLOADER_RESET
};

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


    void printMessage(const QString &message);

    void addResetCommand(unsigned int moduleID);
    void addSetBaseAddress(unsigned int moduleID, unsigned int address);
    void addSendDataInc(unsigned int moduleID, std::vector<unsigned char> &data);

protected:


    NETVInterfaceHandler *m_interface;
    Ui::dsPICBootloader m_ui;
    QList<NETV_MESSAGE> m_msgQueue;


    void generateMessageQueue(hexutils::hex32doc &doc, NetworkModule *module);

    ///QObject event handler overload
    virtual bool event (QEvent * e);

};

#endif

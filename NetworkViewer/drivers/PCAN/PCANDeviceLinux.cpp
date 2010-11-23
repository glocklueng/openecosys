/**
     Copyright (C) 2009-2010 IntRoLab
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
#include "PCANDevice.h"
#include <errno.h>
#include <iostream>
#include <math.h>
#include "CAN_define.h"
#include <QtDebug>

using namespace std;

static bool PCAN_DEVICE_INIT = CANDevice::registerDeviceFactory("PCANDeviceLinux",new CANDevice::DeviceFactory<PCANDevice>("/dev/pcan32","Arguments for Linux only."));


PCANDevice::PCANDevice(const char* device)
    :  m_handle(NULL)
{

    initialize(device);
}

PCANDevice::~PCANDevice()
{

    if (m_handle)
    {
        CAN_Close(m_handle);
    }
}

CANDevice::State PCANDevice::initialize(const char* device)
{

    qDebug("Opening device : %s",device);

    //Open linux device (USB)
    m_handle = LINUX_CAN_Open(const_cast<char*>(device),0);

    if (!m_handle) {
        char message[1024];
        sprintf(message,"PCANDevice::initialize unable to open device  : %s",device);
        perror(message);
        return CANDevice::CANDEVICE_FAIL;
    }

    //Setup device, 1 Mbps, extended frames
    DWORD init_value = CAN_Init(m_handle, CAN_BAUD_1M, CAN_INIT_TYPE_EX);

    if (init_value == 0)
    {
        return CANDevice::CANDEVICE_OK;
    }
    else
    {
        return CANDevice::CANDEVICE_FAIL;
    }
}

bool PCANDevice::newMessageReady()
{

    DWORD status = checkStatus(false);

    return (!(status & CAN_ERR_QRCVEMPTY));

}

DWORD PCANDevice::checkStatus(bool debug){

    //default : can't create resource (default)
    DWORD status = CAN_ERR_RESOURCE;


    if (m_handle)
    {
        //GET STATUS
        status = CAN_Status(m_handle);
    }




    //WRITE STATUS IF IN DEBUG MODE
    if(debug)
    {
        cerr<<"STATUS  ("<<status<<") ";
        if (status & CAN_ERR_OK) // no error
        {
            cerr<<"NO ERROR"<<endl;
        }
        if (status & CAN_ERR_XMTFULL) // transmit buffer full
        {
            cerr<<"TRANSMIT BUFFER FULL!"<<endl;
        }
        if (status & CAN_ERR_OVERRUN)// overrun in receive buffer
        {
            cerr<<"OVERRUN IN RECEIVE BUFFER"<<endl;
        }
        if (status & CAN_ERR_BUSLIGHT)// bus error, errorcounter limit reached
        {
            cerr<<"BUS ERROR, ERRORCOUNTER LIMIT REACHED (BUSLIGHT)"<<endl;
        }
        if (status & CAN_ERR_BUSHEAVY)// bus error, errorcounter limit reached
        {
            cerr<<"BUS ERROR, ERRORCOUNTER LIMIT REACHED (BUSHEAVY)"<<endl;
        }
        if (status & CAN_ERR_BUSOFF)// bus error, 'bus off' state entered
        {
            cerr<<"BUS ERROR, BUS OFF STATE ENTERED"<<endl;
        }
        if (status & CAN_ERR_QRCVEMPTY)// receive queue is empty
        {
            cerr<<"RECEIVE QUEUE IS EMPTY"<<endl;
        }
        if (status & CAN_ERR_QOVERRUN)// receive queue overrun
        {
            cerr<<"RECEIVE QUEUE OVERRUN"<<endl;
        }
        if (status & CAN_ERR_QXMTFULL)// transmit queue full
        {
            cerr<<"TRANSMIT QUEUE FULL"<<endl;
        }
        if (status & CAN_ERR_REGTEST)// test of controller registers failed
        {
            cerr<<"TEST OF CONTROLLER REGISTERS FAILED"<<endl;
        }
        if (status & CAN_ERR_RESOURCE)
            cerr << "0x02000 Resource (FIFO, Client, timeout) cannot be created"<<endl;

        if (status & CAN_ERR_ILLPARAMTYPE)
            cerr << "0x04000 Invalid parameter"<<endl;

        if (status & CAN_ERR_ILLPARAMVAL)
            cerr << "0x08000 Invalid parameter value"<<endl;

	


        cerr<<endl;
    }

    return status;
}


CANDevice::State PCANDevice::sendMessage(LABORIUS_MESSAGE &message) {

    //fill data structure to send
    TPCANMsg msg;

    DWORD result = 0;
    CANDevice::State retval = CANDevice::CANDEVICE_FAIL;

    //ID
    msg.ID = 0;

    //priority
    msg.ID |= (((int)message.msg_priority << 26) & 0x1C000000);

    //type
    msg.ID |= (((int)message.msg_type << 18) & 0x03FC0000);

    //boot
    msg.ID |= (((int)message.msg_boot << 16) & 0x00030000);

    //cmd
    msg.ID |= (((int)message.msg_cmd << 8) & 0x0000FF00);

    //dest
    msg.ID |= (((int)message.msg_dest) & 0x000000FF);

    //MSGTYPE
    if (message.msg_remote) {
        msg.MSGTYPE = MSGTYPE_EXTENDED | MSGTYPE_RTR;
    }
    else {
        msg.MSGTYPE = MSGTYPE_EXTENDED;
    }

    //LEN
    msg.LEN = message.msg_data_length;

    //DATA
    for (int i = 0; i < min((int) message.msg_data_length,8);i++) {
        msg.DATA[i] = message.msg_data[i];
    }

    //will block if send queue is full or until an error happened
    if (m_handle)
    {
        result = CAN_Write(m_handle,&msg);
    }


    retval = CANDevice::CANDEVICE_OK;
    


    return retval;
}



CANDevice::State PCANDevice::recvMessage(LABORIUS_MESSAGE &message) {

    CANDevice::State retval = CANDevice::CANDEVICE_FAIL;
    DWORD result = 0;


    TPCANRdMsg msg;

    if (m_handle)
    {
        //will block for a maximum of 100 us
        result = LINUX_CAN_Read_Timeout(m_handle, &msg, 100);

        if (result == CAN_ERR_OK )
        {

            //PRIORITY
            message.msg_priority = ((int)msg.Msg.ID & 0x1C000000) >> 26;

            //TYPE
            message.msg_type =  ((int)msg.Msg.ID & 0x03FC0000) >> 18;

            //BOOT
            message.msg_boot = ((int)msg.Msg.ID & 0x00030000) >> 16;

            //CMD
            message.msg_cmd  =   ((int)msg.Msg.ID & 0x0000FF00) >> 8;

            //DEST
            message.msg_dest =  ((int)msg.Msg.ID & 0x000000FF);


            //REMOTE
            if (msg.Msg.MSGTYPE  & MSGTYPE_RTR) {
                message.msg_remote = 1;
                message.msg_data_length = 0;
            }
            else {

                message.msg_remote = 0;

                //DATA LENGTH
                message.msg_data_length = msg.Msg.LEN;

                //DATA
                message.msg_data_length = msg.Msg.LEN;
                for (int i = 0; i < min((int)msg.Msg.LEN,8); i++) {
                    message.msg_data[i] = msg.Msg.DATA[i];
                }
            }

            //Set time stamp
            message.msg_dwTime = msg.dwTime;

            applyFilters(message);


            retval = CANDevice::CANDEVICE_OK;

        }//CAN_ERR_OK
    }//if m_handle

    return retval;
}








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
#include "NETV_define.h"
#include <QtDebug>
#include <QDateTime>

using namespace std;

static bool PCAN_DEVICE_INIT = NETVDevice::registerDeviceFactory("PCANDevice",new NETVDevice::DeviceFactory<PCANDevice>("/dev/pcan32","Arguments for Linux only."));


PCANDevice::PCANDevice(const char* device)
    :  m_handle(NULL)
{

#ifdef WIN32
    CAN_Uninitialize(PCAN_USBBUS1);
#endif

    initialize(device);
}

PCANDevice::~PCANDevice()
{

#ifdef WIN32
    CAN_Uninitialize(PCAN_USBBUS1);
#else
    if (m_handle)
    {
        CAN_Close(m_handle);
    }

#endif
}

NETVDevice::State PCANDevice::initialize(const char* device)
{

#ifdef WIN32


    TPCANStatus init_value = CAN_Initialize(
            PCAN_USBBUS1,
            PCAN_BAUD_1M,
            PCAN_MODE_EXTENDED);

    qDebug()<<"PCANDevice::initialize Got init value : "<<init_value;
#else

    qDebug("Opening device : %s",device);

    //Open linux device (USB)
    m_handle = LINUX_CAN_Open(const_cast<char*>(device),0);

    if (!m_handle) {
        char message[1024];
        sprintf(message,"PCANDevice::initialize unable to open device  : %s",device);
        perror(message);
        return NETVDevice::NETVDEVICE_FAIL;
    }

    //Setup device, 1 Mbps, extended frames
    DWORD init_value = CAN_Init(m_handle, CAN_BAUD_1M, CAN_INIT_TYPE_EX);

#endif



    if (init_value == 0)
    {
        return NETVDevice::NETVDEVICE_OK;
    }
    else
    {
        return NETVDevice::NETVDEVICE_FAIL;
    }
}

bool PCANDevice::newMessageReady()
{

    DWORD status = checkStatus(false);

    return (!(status & PCAN_ERROR_QRCVEMPTY));

}

DWORD PCANDevice::checkStatus(bool debug){

    //default : can't create resource (default)
    DWORD status = PCAN_ERROR_RESOURCE;

#ifdef WIN32
    status = CAN_GetStatus(PCAN_USBBUS1);

#else
    if (m_handle)
    {
        //GET STATUS
        status = CAN_Status(m_handle);
    }
#endif



    //WRITE STATUS IF IN DEBUG MODE
    if(debug)
    {
        cerr<<"STATUS  ("<<status<<") ";
        if (status & PCAN_ERROR_OK) // no error
        {
            cerr<<"NO ERROR"<<endl;
        }
        if (status & PCAN_ERROR_XMTFULL) // transmit buffer full
        {
            cerr<<"TRANSMIT BUFFER FULL!"<<endl;
        }
        if (status & PCAN_ERROR_OVERRUN)// overrun in receive buffer
        {
            cerr<<"OVERRUN IN RECEIVE BUFFER"<<endl;
        }
        if (status & PCAN_ERROR_BUSLIGHT)// bus error, errorcounter limit reached
        {
            cerr<<"BUS ERROR, ERRORCOUNTER LIMIT REACHED (BUSLIGHT)"<<endl;
        }
        if (status & PCAN_ERROR_BUSHEAVY)// bus error, errorcounter limit reached
        {
            cerr<<"BUS ERROR, ERRORCOUNTER LIMIT REACHED (BUSHEAVY)"<<endl;
        }
        if (status & PCAN_ERROR_BUSOFF)// bus error, 'bus off' state entered
        {
            cerr<<"BUS ERROR, BUS OFF STATE ENTERED"<<endl;
        }
        if (status & PCAN_ERROR_QRCVEMPTY)// receive queue is empty
        {
            cerr<<"RECEIVE QUEUE IS EMPTY"<<endl;
        }
        if (status & PCAN_ERROR_QOVERRUN)// receive queue overrun
        {
            cerr<<"RECEIVE QUEUE OVERRUN"<<endl;
        }
        if (status & PCAN_ERROR_QXMTFULL)// transmit queue full
        {
            cerr<<"TRANSMIT QUEUE FULL"<<endl;
        }
        if (status & PCAN_ERROR_REGTEST)// test of controller registers failed
        {
            cerr<<"TEST OF CONTROLLER REGISTERS FAILED"<<endl;
        }
        if (status & PCAN_ERROR_NODRIVER)
        {
            cerr<<"NO DRIVER LOADED"<<endl;
        }
        if (status & PCAN_ERROR_HWINUSE)
            cerr<<"Hardware already in use by a Net"<<endl;

        if (status & PCAN_ERROR_NETINUSE)
            cerr<<"0x00800 A Client is already connected to the Net"<<endl;

        if (status & PCAN_ERROR_ILLHW)
            cerr<<"0x01400 Hardware handle is invalid"<<endl;

        if (status & PCAN_ERROR_ILLNET)
            cerr<<"0x01800 Net handle is invalid"<<endl;

        if (status & PCAN_ERROR_ILLCLIENT)
            cerr << "0x01C00 Client handle is invalid"<<endl;

        if (status & PCAN_ERROR_ILLHANDLE)
            cerr <<"(PCAN_ERROR_ILLHW | PCAN_ERROR_ILLNET | PCAN_ERROR_ILLCLIENT) Mask for all handle errors "<<endl;

        if (status & PCAN_ERROR_RESOURCE)
            cerr << "0x02000 Resource (FIFO, Client, timeout) cannot be created"<<endl;

        if (status & PCAN_ERROR_ILLPARAMTYPE)
            cerr << "0x04000 Invalid parameter"<<endl;

        if (status & PCAN_ERROR_ILLPARAMVAL)
            cerr << "0x08000 Invalid parameter value"<<endl;

        if (status & PCAN_ERROR_UNKNOWN)
            cerr << "0x10000 Unknow error"<<endl;

        if (status & PCAN_ERROR_INITIALIZE)
            cerr << "0x40000 Channel is not initialized"<<endl;


        cerr<<endl;
    }

    return status;
}


NETVDevice::State PCANDevice::sendMessage(NETV_MESSAGE &message) {

    //fill data structure to send
    TPCANMsg msg;

    DWORD result = 0;
    NETVDevice::State retval = NETVDevice::NETVDEVICE_FAIL;

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
        msg.MSGTYPE = PCAN_MESSAGE_EXTENDED | PCAN_MESSAGE_RTR;
    }
    else {
        msg.MSGTYPE = PCAN_MESSAGE_EXTENDED;
    }

    //LEN
    msg.LEN = message.msg_data_length;

    //DATA
    for (int i = 0; i < min((int) message.msg_data_length,8);i++) {
        msg.DATA[i] = message.msg_data[i];
    }

    //will block if send queue is full or until an error happened
#ifdef WIN32
    result = CAN_Write(PCAN_USBBUS1,&msg);
#else
    if (m_handle)
    {
        result = CAN_Write(m_handle,&msg);
    }
#endif

    retval = NETVDevice::NETVDEVICE_OK;
    


    return retval;
}



NETVDevice::State PCANDevice::recvMessage(NETV_MESSAGE &message) {

    NETVDevice::State retval = NETVDevice::NETVDEVICE_FAIL;
    DWORD result = 0;

#ifdef WIN32
    TPCANMsg msg;
    TPCANTimestamp stamp;

    result = CAN_Read(PCAN_USBBUS1, &msg,&stamp);

    //qDebug("GOT READ RESULT %i",result);

    if (result == PCAN_ERROR_OK )
    {
        switch (msg.MSGTYPE)
        {
        case PCAN_MESSAGE_EXTENDED:
            //PRIORITY
            message.msg_priority = ((int)msg.ID & 0x1C000000) >> 26;

            //TYPE
            message.msg_type =  ((int)msg.ID & 0x03FC0000) >> 18;

            //BOOT
            message.msg_boot = ((int)msg.ID & 0x00030000) >> 16;

            //CMD
            message.msg_cmd  =   ((int)msg.ID & 0x0000FF00) >> 8;

            //DEST
            message.msg_dest =  ((int)msg.ID & 0x000000FF);

            //REMOTE
            if (msg.MSGTYPE  & PCAN_MESSAGE_RTR) {
                message.msg_remote = 1;
                message.msg_data_length = 0;
            }
            else {

                message.msg_remote = 0;

                //DATA LENGTH
                message.msg_data_length = msg.LEN;

                //DATA
                message.msg_data_length = msg.LEN;
                for (int i = 0; i < min((int)msg.LEN,8); i++) {
                    message.msg_data[i] = msg.DATA[i];
                }
            }

            applyFilters(message);

            //Set time stamp
            message.msg_timestamp = QDateTime::currentDateTime().toMSecsSinceEpoch();

            applyFilters(message);


            break;//EXTENDED FRAME DETECTED
                        case PCAN_MESSAGE_STANDARD:
            qDebug("MSGTYPE_STANDARD NOT SUPPORTED");
            break;

                        case PCAN_MESSAGE_STATUS:
            qDebug("MSGTYPE_STATUS");
            break;
        }//switch message type

        retval = NETVDevice::NETVDEVICE_OK;
    }//result == ok
    else if (result == PCAN_ERROR_QRCVEMPTY)
    {
        retval = NETVDevice::NETVDEVICE_UNDERFLOW;
    }
    else if (result == PCAN_ERROR_BUSLIGHT || result == PCAN_ERROR_BUSHEAVY || result == PCAN_ERROR_BUSOFF)
    {
        retval = NETVDevice::NETVDEVICE_BUS;
    }

#else //LINUX...
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


            retval = NETVDevice::NETVDEVICE_OK;

        }//CAN_ERR_OK
    }//if m_handle
#endif
    return retval;
}








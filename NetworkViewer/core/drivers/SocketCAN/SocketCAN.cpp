/**
     Copyright 2011 (C) Dominic Letourneau, ing. M.Sc.A.
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

#include "SocketCAN.h"
#include <QDebug>
#include <QCoreApplication>
#include <QTime>
#include <math.h>

static bool SOCKET_CAN_DEVICE_INIT = NETVDevice::registerDeviceFactory("SocketCAN",new NETVDevice::DeviceFactory<SocketCAN>("can0","The device to use."));


SocketCAN::SocketCAN(const char* args)
    :	m_fd(-1)
{

    initialize(args);
}

SocketCAN::~SocketCAN()
{
    if (m_fd > 0)
    {
        close(m_fd);
    }
}

NETVDevice::State SocketCAN::initialize(const char* args)
{

    QStringList argsList = QString(args).split(";");

    if (argsList.size() == 1)
    {
	//Let's try to open the device
	struct sockaddr_can addr;
	struct ifreq ifr;

	//Open device
	m_fd = socket(PF_CAN,SOCK_RAW,CAN_RAW);
	if (m_fd < 0)
	{
            perror("socket : ");
            return NETVDevice::NETVDEVICE_FAIL;
	}


	//Set non blocking socket
        fcntl(m_fd, F_SETFL , O_NONBLOCK); 


	//Get device index
	strcpy(ifr.ifr_name,argsList[0].toStdString().c_str());
	ioctl(m_fd,SIOCGIFINDEX, &ifr);
	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;

	//Bind the socket
	if(bind(m_fd,(struct sockaddr*) &addr, sizeof(addr)) < 0)
	{
            perror("bind : ");
            close(m_fd);
            m_fd = -1;
            return NETVDevice::NETVDEVICE_FAIL;
	}

	qDebug("NETVDevice::State SocketCAN::initialize with args: %s, OK!",args);
	return NETVDevice::NETVDEVICE_OK;

    }
    else
    {
	return NETVDevice::NETVDEVICE_FAIL;
    }
    
}

NETVDevice::State SocketCAN::sendMessage(NETV_MESSAGE &message)
{
    //BE CAREFUL THIS FUNCTION RUNS IN ANOTHER THREAD
    //qDebug("NETVDevice::State SocketCAN::sendMessage(NETV_MESSAGE &message)");

    if (m_fd > 0)
    {
	/*
	 struct can_frame {
		     canid_t can_id;  // 32 bit CAN_ID + EFF/RTR/ERR flags 
		     __u8    can_dlc; // data length code: 0 .. 8 
		     __u8    data[8] __attribute__((aligned(8)));
	     };
	*/

	//Writing to the socket
	struct can_frame frame;

	frame.can_id = 0;

        //SID + EID
        frame.can_id =  ((unsigned int)(message.msg_priority) << 26) & 0x1C000000;
        frame.can_id |= ((unsigned int)(message.msg_type) << 18) & 0x03FC0000;
        frame.can_id |= ((unsigned int)(message.msg_boot) << 16) & 0x00030000;
        frame.can_id |= ((unsigned int)(message.msg_cmd) << 8) & 0x0000FF00;
        frame.can_id |= ((unsigned int)(message.msg_dest)) & 0x000000FF;

        //RTR
        if (message.msg_remote)
	{
            frame.can_id |= CAN_RTR_FLAG;
	}       

        //EFF (extended frame flag)
        frame.can_id |= CAN_EFF_FLAG;

	
	//Data length
	frame.can_dlc = message.msg_data_length;

	//Copy data
	for (int i = 0; i < std::min(8,(int)message.msg_data_length); i++)
	{
            frame.data[i] = message.msg_data[i];
	}

	int nbytes = write(m_fd, &frame, sizeof(struct can_frame));

	if (nbytes == sizeof(struct can_frame))
        {
            return NETVDevice::NETVDEVICE_OK;

	}

	perror("NETVDevice::State SocketCAN::sendMessage(NETV_MESSAGE &message) - Error sending frame");
	
	//We have processed the message
	return NETVDevice::NETVDEVICE_FAIL;
    }
    else
    {
	return NETVDevice::NETVDEVICE_NOT_INITIALIZED;
    }
}

NETVDevice::State SocketCAN::recvMessage(NETV_MESSAGE &message)
{
    //BE CAREFUL THIS FUNCTION RUNS IN ANOTHER THREAD


    //qDebug("NETVDevice::State SocketCAN::recvMessage(NETV_MESSAGE &message)");

    if (m_fd > 0)
    {


	/*
	 struct can_frame {
		     canid_t can_id;  // 32 bit CAN_ID + EFF/RTR/ERR flags 
		     __u8    can_dlc; // data length code: 0 .. 8 
		     __u8    data[8] __attribute__((aligned(8)));
	     };
	*/

	struct can_frame frame;

        int nbytes = read(m_fd, &frame, sizeof(struct can_frame));

   	if (nbytes <= 0) 
	{
            //perror("NETVDevice::State SocketCAN::recvMessage(NETV_MESSAGE &message) - can raw socket read");
            return NETVDevice::NETVDEVICE_UNDERFLOW;
	}
	else
	{

            //PRIORITY
            message.msg_priority = (frame.can_id & 0x1C000000) >> 26;

            //TYPE
            message.msg_type =  (frame.can_id & 0x03FC0000) >> 18;

            //BOOT
            message.msg_boot = (frame.can_id & 0x00030000) >> 16;

            //CMD
            message.msg_cmd  =   (frame.can_id & 0x0000FF00) >> 8;

            //DEST
            message.msg_dest =  (frame.can_id& 0x000000FF);

            //DATA LENGTH
            message.msg_data_length = frame.can_dlc;

            //DATA
            memcpy(message.msg_data,frame.data,frame.can_dlc);

            if (frame.can_id & CAN_RTR_FLAG)
            {
                message.msg_remote = 1;
            }


            return NETVDevice::NETVDEVICE_OK;
	}

    }

    return NETVDevice::NETVDEVICE_NOT_INITIALIZED;;
}

bool SocketCAN::newMessageReady()
{
    return false;
}



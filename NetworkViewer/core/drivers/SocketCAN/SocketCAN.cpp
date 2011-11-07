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
{

    initialize(args);
}

SocketCAN::~SocketCAN()
{

}

NETVDevice::State SocketCAN::initialize(const char* args)
{

    int count = QString(args).toInt();



    return NETVDevice::NETVDEVICE_OK;
}

NETVDevice::State SocketCAN::sendMessage(NETV_MESSAGE &message)
{
    //BE CAREFUL THIS FUNCTION RUNS IN ANOTHER THREAD

    //We have processed the message
    return NETVDevice::NETVDEVICE_OK;
}

NETVDevice::State SocketCAN::recvMessage(NETV_MESSAGE &message)
{
    //BE CAREFUL THIS FUNCTION RUNS IN ANOTHER THREAD
    NETVDevice::State returnState = NETVDevice::NETVDEVICE_FAIL;




    return returnState;
}

bool SocketCAN::newMessageReady()
{
    return false;
}



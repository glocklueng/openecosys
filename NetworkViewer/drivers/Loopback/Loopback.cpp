/**
     Copyright (C) 2011 Dominic Letourneau, ing. M.Sc.A.
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

#include "Loopback.h"
#include <QDebug>
#include <QCoreApplication>

static bool LOOPBACK_DEVICE_INIT = NETVDevice::registerDeviceFactory("Loopback",new NETVDevice::DeviceFactory<Loopback>("COM4;1000000","SerialPort;speed. To be used with PIC32 Module."));


Loopback::Loopback(const char* args)
{

    initialize(args);
}

Loopback::~Loopback()
{

}

NETVDevice::State Loopback::initialize(const char* args)
{
	return NETVDevice::NETVDEVICE_OK;
}

NETVDevice::State Loopback::sendMessage(NETV_MESSAGE &message)
{
	//BE CAREFUL THIS FUNCTION RUNS IN ANOTHER THREAD
	return NETVDevice::NETVDEVICE_FAIL;
}

NETVDevice::State Loopback::recvMessage(NETV_MESSAGE &message)
{
    //BE CAREFUL THIS FUNCTION RUNS IN ANOTHER THREAD
    NETVDevice::State returnState = NETVDevice::NETVDEVICE_FAIL;

   

    return returnState;
}


bool Loopback::event(QEvent *event)
{

    if(event->type() == QEvent::User)
    {

       

            return true;
        
    }

    return QObject::event(event);
}



bool Loopback::newMessageReady()
{
	return false;
}


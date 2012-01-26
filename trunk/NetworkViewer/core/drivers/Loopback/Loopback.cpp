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

#include "Loopback.h"
#include <QDebug>
#include <QCoreApplication>
#include <QTime>
#include <math.h>

static bool LOOPBACK_DEVICE_INIT = NETVDevice::registerDeviceFactory("Loopback",new NETVDevice::DeviceFactory<Loopback>("5","The number of virtual modules."));


Loopback::Loopback(const char* args)
{
    m_time = QTime::currentTime();
    m_time.start();
    initialize(args);
}

Loopback::~Loopback()
{

}

NETVDevice::State Loopback::initialize(const char* args)
{

    int count = QString(args).toInt();

    //create virtual modules
    for (unsigned int i= 0; i < count; i++)
    {
        m_moduleList.push_back(VirtualModule(i));
    }

    if (count > 0)
    {
        m_updateTimer = new QTimer(this);
        connect(m_updateTimer,SIGNAL(timeout()),this,SLOT(timeout()));
        m_updateTimer->start(50);
    }

    return NETVDevice::NETVDEVICE_OK;
}

NETVDevice::State Loopback::sendMessage(NETV_MESSAGE &message)
{

    message.msg_timestamp = QDateTime::currentDateTime().toMSecsSinceEpoch();

    //BE CAREFUL THIS FUNCTION RUNS IN ANOTHER THREAD
    if (message.msg_type == NETV_TYPE_REQUEST_DATA)
    {
        int device_id = message.msg_dest;

        m_mutex.lock();

        if (device_id < m_moduleList.size())
        {
            //Reading
            if (message.msg_remote)
            {
                NETV_MESSAGE answer = message;
                if (message.msg_cmd + message.msg_data_length <= (VirtualModule::NB_VARIABLES * sizeof(double)))
                {
                    memcpy(answer.msg_data,&m_moduleList[device_id].data[message.msg_cmd],message.msg_data_length);
                    answer.msg_remote = 0;
                    //Writing answer
                    m_messageList.push_back(answer);
                    m_semaphore.release(1);
                }
            }
            else //Writing
            {
                if (message.msg_cmd + message.msg_data_length <= (VirtualModule::NB_VARIABLES * sizeof(double)))
                {
                    memcpy(&m_moduleList[device_id].data[message.msg_cmd],message.msg_data,message.msg_data_length);
                }
            }
        }
        m_mutex.unlock();

    }
    else if (message.msg_type == NETV_TYPE_EVENTS)
    {
        if (message.msg_cmd == NETV_EVENTS_CMD_ALIVE)
        {
            m_mutex.lock();

            //process virtual modules alive request
            for(unsigned int i = 0; i < m_moduleList.size(); i++)
            {
                NETV_MESSAGE answer = message;

                answer.msg_remote = 0;

                //Fill data bytes
                answer.msg_data[0] = m_moduleList[i].state;
                answer.msg_data[1] = m_moduleList[i].project_id;
                answer.msg_data[2] = m_moduleList[i].module_id;
                answer.msg_data[3] = m_moduleList[i].code_version;
                answer.msg_data[4] = m_moduleList[i].table_version;
                answer.msg_data[5] = 0; //boot delay
                answer.msg_data[6] = m_moduleList[i].device_id >> 8; //device id
                answer.msg_data[7] = m_moduleList[i].device_id; //devicd id

                //Push back answer
                m_messageList.push_back(answer);
            }

            m_mutex.unlock();

            //Signal semaphore
            m_semaphore.release(m_moduleList.size());
        }

    }
    else if (message.msg_type == NETV_TYPE_BOOTLOADER)
    {

        unsigned int device_id = message.msg_dest;

        if (device_id < m_moduleList.size())
        {

            if(m_moduleList[device_id].state == NETV_BOOT_MODE_ID)
            {
                //Send back bootloader ACK...
                m_mutex.lock();
                //return message
                m_messageList.push_back(message);
                m_mutex.unlock();
                m_semaphore.release(1);


                //Must be in boot mode to answer bootloader
                if (message.msg_cmd == BOOTLOADER_RESET)
                {
                    qDebug("Loopback::sendMessage BOOTLOADER_RESET");
                    m_moduleList[device_id].state = NETV_NORMAL_MODE_ID;
                }
            }
        }
    }
    else if (message.msg_type == NETV_TYPE_EMERGENCY)
    {
        unsigned int device_id = message.msg_dest;

        if (device_id < m_moduleList.size())
        {
            if (message.msg_cmd == NETV_EMERGENCY_CMD_RESET)
            {
                qDebug("Loopback::sendMessage NETV_EMERGENCY_CMD_RESET");
                m_moduleList[device_id].state = NETV_NORMAL_MODE_ID;
            }
            else if (message.msg_cmd == NETV_EMERGENCY_CMD_PROGRAM)
            {
                qDebug("Loopback::sendMessage NETV_EMERGENCY_CMD_PROGRAM");
                m_moduleList[device_id].state = NETV_BOOT_MODE_ID;
            }
        }
    }

    //We have processed the message
    return NETVDevice::NETVDEVICE_OK;
}

NETVDevice::State Loopback::recvMessage(NETV_MESSAGE &message)
{
    //BE CAREFUL THIS FUNCTION RUNS IN ANOTHER THREAD
    NETVDevice::State returnState = NETVDevice::NETVDEVICE_FAIL;

    bool acquire = m_semaphore.tryAcquire(1,10);

    if (acquire)
    {
        m_mutex.lock();
        message = m_messageList.front();
        m_messageList.pop_front();
        m_mutex.unlock();
        returnState = NETVDevice::NETVDEVICE_OK;
    }
    else
    {
        returnState = NETVDevice::NETVDEVICE_UNDERFLOW;
    }


    return returnState;
}

bool Loopback::newMessageReady()
{
    return (m_semaphore.available() > 0);
}

void Loopback::timeout()
{

    //Generate sinus
    m_mutex.lock();

    double elapsed = m_time.elapsed() / 1000.0;

    for (unsigned int i = 0; i < m_moduleList.size(); i++)
    {
        for(unsigned int j = 0; j < VirtualModule::NB_SINE_VARIABLES; j++)
        {
            m_moduleList[i].variable[j] = 100 * sin(2 * M_PI * (j / 10.0 + 0.01) * elapsed);
        }
    }

    m_mutex.unlock();

}

/**
 */

#include "Loopback.h"
#include <QDebug>
#include <QCoreApplication>

static bool LOOPBACK_DEVICE_INIT = NETVDevice::registerDeviceFactory("Loopback",new NETVDevice::DeviceFactory<Loopback>("5","The number of virtual modules."));


Loopback::Loopback(const char* args)
{

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

    return NETVDevice::NETVDEVICE_OK;
}

NETVDevice::State Loopback::sendMessage(NETV_MESSAGE &message)
{
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
                memcpy(answer.msg_data,&m_moduleList[device_id].data[message.msg_cmd],message.msg_data_length);
                answer.msg_remote = 0;
                //Writing answer
                m_messageList.push_back(answer);
                m_semaphore.release(1);
            }
            else //Writing
            {
                memcpy(&m_moduleList[device_id].data[message.msg_cmd],message.msg_data,message.msg_data_length);
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


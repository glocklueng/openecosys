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

#include "NETVInterfaceHandler.h"




NETVInterfaceHandler::NETVRecvThread::NETVRecvThread(NETVInterfaceHandler *handler)
   : QThread(handler), m_handler(handler)
{

}


void NETVInterfaceHandler::NETVRecvThread::run ()
{
        qDebug("Starting NETV recv thread");

	while(m_handler->isRunning())
	{
		m_handler->recvThreadFunction();
	}

        qDebug("Stopping NETV recv thread");

	//Event loop (will handle thread termination)
	//exec();

        qDebug("Done NETV recv thread");
}


void NETVInterfaceHandler::NETVRecvThread::thread_usleep(unsigned long microseconds)
{
	usleep(microseconds);
}

NETVInterfaceHandler::NETVSendThread::NETVSendThread(NETVInterfaceHandler *handler)
	: QThread(handler), m_handler(handler)
{

}

void NETVInterfaceHandler::NETVSendThread::run ()
{
        qDebug("Starting NETV send thread");

	while(m_handler->isRunning())
	{
		m_handler->sendThreadFunction();
	}

        qDebug("Stopping NETV send thread");

	//event loop (will handle thread termination)
	//exec();

        qDebug("Done NETV send thread");
}

void NETVInterfaceHandler::NETVSendThread::thread_usleep(unsigned long microseconds)
{
	usleep(microseconds);
}



/*******************
 *
 *
 ********************/
NETVInterfaceHandler::NETVInterfaceHandler(NETVDevice *device, const char* args, QObject *parent)
        : QObject(parent), m_device(device), m_running(false), m_NETVSendThread(NULL), m_NETVRecvThread(NULL), m_messageSent(0), m_messageReceived(0)
{

	if (m_device)
	{
                qDebug("NETVInterfaceHandler::NETVInterfaceHandler with device : %p",m_device);

		//Change the state to running
		m_running = true;

		//Start worker threads
                m_NETVSendThread = new NETVSendThread(this);
                connect(m_NETVSendThread, SIGNAL(finished()),
				this, SLOT(sendThreadFinished()));
                m_NETVSendThread->start(QThread::TimeCriticalPriority);

                m_NETVRecvThread = new NETVRecvThread(this);
                connect(m_NETVRecvThread, SIGNAL(finished()),
				this, SLOT(recvThreadFinished()));
                m_NETVRecvThread->start(QThread::TimeCriticalPriority);
	}
	else
	{
                qDebug("NETVInterfaceHandler() Cannot open NETV device");
	}
}

///Destructor, will terminate threads and close the NETV device
NETVInterfaceHandler::~NETVInterfaceHandler()
{

        qDebug("~NETVInterfaceHandler() Destroying threads.");
	m_running = false;

        if (m_NETVSendThread)
	{
                m_NETVSendThread->exit();
		m_listSemaphore.release(1);
                m_NETVSendThread->wait();
                qDebug("~NETVInterfaceHandler() Destroying threads send (ok)");
	}

        if (m_NETVRecvThread)
	{
                m_NETVRecvThread->exit();
		m_listSemaphore.release(1);
                m_NETVRecvThread->wait();
                qDebug("~NETVInterfaceHandler() Destroying threads recv (ok)");
	}


        qDebug("~NETVInterfaceHandler() Threads all terminated. Destroying device : %p",m_device);

        if (m_device)
        {
            delete m_device;
        }

        ///This will close the NETV device
	//m_device->close();
}


bool NETVInterfaceHandler::isRunning()
{
	return m_running;
}


void NETVInterfaceHandler::registerObserver(NETVMessageObserverIF *observer)
{
	m_observerMutex.lock();
	m_observers.push_back(observer);
	m_observerMutex.unlock();
}


void NETVInterfaceHandler::unregisterObserver(NETVMessageObserverIF *observer)
{
	m_observerMutex.lock();
        for (std::list<NETVMessageObserverIF*>::iterator iter = m_observers.begin(); iter != m_observers.end(); iter++)
	{
		if (*iter == observer)
		{
			m_observers.erase(iter);
			break;
		}
	}
	m_observerMutex.unlock();
}


void NETVInterfaceHandler::pushNETVMessage(const NETV_MESSAGE &msg)
{
        if (m_NETVSendThread)
	{
		m_listMutex.lock();
		m_msgQueue.push_back(msg);
		//PCANDevice::printMessage(m_msgQueue.back(),cerr);
		m_listMutex.unlock();
		m_listSemaphore.release(1);
	}
}


void NETVInterfaceHandler::pushNETVMessages(const std::list<NETV_MESSAGE> &msgList)
{
	for (std::list<NETV_MESSAGE>::const_iterator iter = msgList.begin(); iter != msgList.end(); iter++)
	{
                pushNETVMessage(*iter);
	}
}

void NETVInterfaceHandler::sendThreadFunction()
{
	NETVDevice::State result;

	//WAIT UNTIL WE HAVE SOMETHING TO SEND
	m_listSemaphore.acquire(1);

	//PROTECT THE QUEUE
	m_listMutex.lock();

	//get message, safety check the size of the list and if we are running
	if (m_msgQueue.size() > 0 && m_running)
	{
		//send it
		if (m_device)
		{
			result = m_device->sendMessage(m_msgQueue.front());
		}

                if (result == NETVDevice::NETVDEVICE_OK)
		{
                        m_messageSent++;
                        //qDebug("NETVInterfaceHandler::sendThreadFunction() result == NETVDEVICE_OK");
			m_msgQueue.pop_front();
		}
                else if (result == NETVDevice::NETVDEVICE_OVERFLOW)
		{
                        //Do we need to unlock the list here?

                        qDebug("NETVInterfaceHandler::sendThreadFunction() result == NETVDEVICE_OVERFLOW");
                        m_NETVSendThread->thread_usleep(1000);

                        //signal semaphore for retry
                        m_listSemaphore.release(1);
		}
                else
                {
                    qDebug("NETVInterfaceHandler::sendThreadFunction() Unhandled state result = %i",result);
                }
	}

	//UNPROTECT THE QUEUE
	m_listMutex.unlock();

        if (result == NETVDevice::NETVDEVICE_NOT_INITIALIZED)
	{
                qDebug("NETVInterfaceHandler::sendThreadFunction() result == NETVDEVICE_NOT_INITIALIZED, stopping send thread.");
                m_NETVSendThread->quit();
	}
}


void NETVInterfaceHandler::recvThreadFunction()
{
	if (m_device)
	{
		NETV_MESSAGE recvMessage;

		//THIS SHOULD BE BLOCKING
		NETVDevice::State result = m_device->recvMessage(recvMessage);

                if (result == NETVDevice::NETVDEVICE_OK)
		{
                        m_messageReceived++;
                        //qDebug("NETVInterfaceHandler::recvThreadFunction() result == NETVDEVICE_OK, stopping recv thread.");
			//NOTIFY OBSERVERS THAT WE HAVE A NEW MESSAGE
			notifyCANObservers(recvMessage);
		}
                else if (result == NETVDevice::NETVDEVICE_UNDERFLOW)
		{
                        //qDebug("NETVInterfaceHandler::recvThreadFunction() result == NETVDEVICE_UNDERFLOW, stopping recv thread.");
                        m_NETVRecvThread->thread_usleep(1000);//will sleep 1000us
		}
                else if (result == NETVDevice::NETVDEVICE_BUS)
		{
                        m_NETVRecvThread->thread_usleep(1000);//will sleep 1000us
		}
                else if (result == NETVDevice::NETVDEVICE_FAIL)
		{
                        //qDebug("NETVInterfaceHandler::recvThreadFunction() result == NETVDEVICE_FAIL.");
                        m_NETVRecvThread->thread_usleep(1000);//will sleep 1000us
			//m_running = false;
		}
                else
                {
                    qDebug("NETVInterfaceHandler::recvThreadFunction() : Unhandled state result = %i",result);
                    m_NETVRecvThread->thread_usleep(1000);//will sleep 1000us
                }
	}
}


void NETVInterfaceHandler::notifyCANObservers(const NETV_MESSAGE &msg)
{
	//MAKE SURE WE PROTECT THE LIST OF OBSERVERS
	m_observerMutex.lock();

        for (std::list<NETVMessageObserverIF*>::iterator iter = m_observers.begin(); iter != m_observers.end(); iter++)
	{
                (*iter)->notifyNETVMessage(msg);
	}
	m_observerMutex.unlock();
}

void NETVInterfaceHandler::sendThreadFinished()
{
        qDebug("NETVInterfaceHandler::sendThreadFinished()");
        m_NETVSendThread->wait();
        delete m_NETVSendThread;
        m_NETVSendThread = NULL;
}

void NETVInterfaceHandler::recvThreadFinished()
{
        qDebug("NETVInterfaceHandler::recvThreadFinished()");
        m_NETVRecvThread->wait();
        delete m_NETVRecvThread;
        m_NETVRecvThread = NULL;
}



int NETVInterfaceHandler::messageReceivedCounter()
{
    return m_messageReceived;
}


int NETVInterfaceHandler::messageSentCounter()
{
    return m_messageSent;
}

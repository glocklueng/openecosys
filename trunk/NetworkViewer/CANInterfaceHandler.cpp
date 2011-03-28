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

#include "CANInterfaceHandler.h"




CANInterfaceHandler::CANRecvThread::CANRecvThread(CANInterfaceHandler *handler)
   : QThread(handler), m_handler(handler)
{

}


void CANInterfaceHandler::CANRecvThread::run ()
{
	qDebug("Starting CAN recv thread");

	while(m_handler->isRunning())
	{
		m_handler->recvThreadFunction();
	}

	qDebug("Stopping CAN recv thread");

	//Event loop (will handle thread termination)
	//exec();

	qDebug("Done CAN recv thread");
}


void CANInterfaceHandler::CANRecvThread::thread_usleep(unsigned long microseconds)
{
	usleep(microseconds);
}

CANInterfaceHandler::CANSendThread::CANSendThread(CANInterfaceHandler *handler)
	: QThread(handler), m_handler(handler)
{

}

void CANInterfaceHandler::CANSendThread::run ()
{
	qDebug("Starting CAN send thread");

	while(m_handler->isRunning())
	{
		m_handler->sendThreadFunction();
	}

	qDebug("Stopping CAN send thread");

	//event loop (will handle thread termination)
	//exec();

	qDebug("Done CAN send thread");
}

void CANInterfaceHandler::CANSendThread::thread_usleep(unsigned long microseconds)
{
	usleep(microseconds);
}



/*******************
 *
 *
 ********************/
CANInterfaceHandler::CANInterfaceHandler(CANDevice *device, const char* args, QObject *parent)
        : QObject(parent), m_device(device), m_running(false), m_canSendThread(NULL), m_canRecvThread(NULL), m_messageSent(0), m_messageReceived(0)
{

	if (m_device)
	{
		qDebug("CANInterfaceHandler::CANInterfaceHandler with device : %p",m_device);

		//Change the state to running
		m_running = true;

		//Start worker threads
		m_canSendThread = new CANSendThread(this);
		connect(m_canSendThread, SIGNAL(finished()),
				this, SLOT(sendThreadFinished()));
		m_canSendThread->start(QThread::TimeCriticalPriority);

		m_canRecvThread = new CANRecvThread(this);
		connect(m_canRecvThread, SIGNAL(finished()),
				this, SLOT(recvThreadFinished()));
		m_canRecvThread->start(QThread::TimeCriticalPriority);
	}
	else
	{
		qDebug("CANInterfaceHandler() Cannot open CAN device");
	}
}

///Destructor, will terminate threads and close the CAN device
CANInterfaceHandler::~CANInterfaceHandler()
{

	qDebug("~CANInterfaceHandler() Destroying threads.");
	m_running = false;

	if (m_canSendThread)
	{
		m_canSendThread->exit();
		m_listSemaphore.release(1);
		m_canSendThread->wait();
		qDebug("~CANInterfaceHandler() Destroying threads send (ok)");
	}

	if (m_canRecvThread)
	{
		m_canRecvThread->exit();
		m_listSemaphore.release(1);
		m_canRecvThread->wait();
		qDebug("~CANInterfaceHandler() Destroying threads recv (ok)");
	}


	qDebug("~CANInterfaceHandler() Threads all terminated. Destroying device : %p",m_device);

        if (m_device)
        {
            delete m_device;
        }

	///This will close the CAN device
	//m_device->close();
}


bool CANInterfaceHandler::isRunning()
{
	return m_running;
}


void CANInterfaceHandler::registerObserver(CANMessageObserverIF *observer)
{
	m_observerMutex.lock();
	m_observers.push_back(observer);
	m_observerMutex.unlock();
}


void CANInterfaceHandler::unregisterObserver(CANMessageObserverIF *observer)
{
	m_observerMutex.lock();
	for (std::list<CANMessageObserverIF*>::iterator iter = m_observers.begin(); iter != m_observers.end(); iter++)
	{
		if (*iter == observer)
		{
			m_observers.erase(iter);
			break;
		}
	}
	m_observerMutex.unlock();
}


void CANInterfaceHandler::pushCANMessage(const LABORIUS_MESSAGE &msg)
{
	if (m_canSendThread)
	{
		m_listMutex.lock();
		m_msgQueue.push_back(msg);
		//PCANDevice::printMessage(m_msgQueue.back(),cerr);
		m_listMutex.unlock();
		m_listSemaphore.release(1);
	}
}


void CANInterfaceHandler::pushCANMessages(const std::list<LABORIUS_MESSAGE> &msgList)
{
	for (std::list<LABORIUS_MESSAGE>::const_iterator iter = msgList.begin(); iter != msgList.end(); iter++)
	{
		pushCANMessage(*iter);
	}
}

void CANInterfaceHandler::sendThreadFunction()
{
	CANDevice::State result;

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

		if (result == CANDevice::CANDEVICE_OK)
		{
                        m_messageSent++;
			//qDebug("CANInterfaceHandler::sendThreadFunction() result == CANDEVICE_OK");
			m_msgQueue.pop_front();
		}
		else if (result == CANDevice::CANDEVICE_OVERFLOW)
		{
                        //Do we need to unlock the list here?

			qDebug("CANInterfaceHandler::sendThreadFunction() result == CANDEVICE_OVERFLOW");
                        m_canSendThread->thread_usleep(1000);

                        //signal semaphore for retry
                        m_listSemaphore.release(1);
		}
                else
                {
                    qDebug("CANInterfaceHandler::sendThreadFunction() Unhandled state result = %i",result);
                }
	}

	//UNPROTECT THE QUEUE
	m_listMutex.unlock();

	if (result == CANDevice::CANDEVICE_NOT_INITIALIZED)
	{
		qDebug("CANInterfaceHandler::sendThreadFunction() result == CANDEVICE_NOT_INITIALIZED, stopping send thread.");
		m_canSendThread->quit();
	}
}


void CANInterfaceHandler::recvThreadFunction()
{
	if (m_device)
	{
		LABORIUS_MESSAGE recvMessage;

		//THIS SHOULD BE BLOCKING
		CANDevice::State result = m_device->recvMessage(recvMessage);

		if (result == CANDevice::CANDEVICE_OK)
		{
                        m_messageReceived++;
			//qDebug("CANInterfaceHandler::recvThreadFunction() result == CANDEVICE_OK, stopping recv thread.");
			//NOTIFY OBSERVERS THAT WE HAVE A NEW MESSAGE
			notifyCANObservers(recvMessage);
		}
		else if (result == CANDevice::CANDEVICE_UNDERFLOW)
		{
			//qDebug("CANInterfaceHandler::recvThreadFunction() result == CANDEVICE_UNDERFLOW, stopping recv thread.");
                        m_canRecvThread->thread_usleep(1000);//will sleep 1000us
		}
		else if (result == CANDevice::CANDEVICE_BUS)
		{
                        m_canRecvThread->thread_usleep(1000);//will sleep 1000us
		}
		else if (result == CANDevice::CANDEVICE_FAIL)
		{
                        //qDebug("CANInterfaceHandler::recvThreadFunction() result == CANDEVICE_FAIL.");
                        m_canRecvThread->thread_usleep(1000);//will sleep 1000us
			//m_running = false;
		}
                else
                {
                    qDebug("CANInterfaceHandler::recvThreadFunction() : Unhandled state result = %i",result);
                    m_canRecvThread->thread_usleep(1000);//will sleep 1000us
                }
	}
}


void CANInterfaceHandler::notifyCANObservers(const LABORIUS_MESSAGE &msg)
{
	//MAKE SURE WE PROTECT THE LIST OF OBSERVERS
	m_observerMutex.lock();

	for (std::list<CANMessageObserverIF*>::iterator iter = m_observers.begin(); iter != m_observers.end(); iter++)
	{
		(*iter)->notifyCANMessage(msg);
	}
	m_observerMutex.unlock();
}

void CANInterfaceHandler::sendThreadFinished()
{
	qDebug("CANInterfaceHandler::sendThreadFinished()");
	m_canSendThread->wait();
	delete m_canSendThread;
	m_canSendThread = NULL;
}

void CANInterfaceHandler::recvThreadFinished()
{
	qDebug("CANInterfaceHandler::recvThreadFinished()");
	m_canRecvThread->wait();
	delete m_canRecvThread;
	m_canRecvThread = NULL;
}



int CANInterfaceHandler::messageReceivedCounter()
{
    return m_messageReceived;
}


int CANInterfaceHandler::messageSentCounter()
{
    return m_messageSent;
}

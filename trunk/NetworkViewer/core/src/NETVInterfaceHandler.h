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

#ifndef _NETV_INTERFACE_HANDLER_H_
#define _NETV_INTERFACE_HANDLER_H_

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QSemaphore>
#include "NETVDevice.h"
#include <QCoreApplication>
#include <QTimer>



/**
   This interface will allow us to receive NETV messages if we subscribe to
   the service. Right now messages are not filtered, and we will receive
   all the NETV messages on the bus if we subscribe to the service. You must derive
   from this class to be able to receive messages.

    \author Dominic Letourneau
    \date 22/05/2008
*/
class NETVMessageObserverIF
{
    public:
    ///notify function for new messages
    virtual void notifyNETVMessage(const NETV_MESSAGE &msg) = 0;
};


/**
    NETVInterfaceHandler will handle NETV communication. This class is thread safe.
    A receive and a send thread will be created for communication.
    \author Dominic Letourneau
    \date 22/05/2008
*/
class NETVInterfaceHandler : public QObject
{
    Q_OBJECT;

    public:


    ///WORKER THREADS ARE FRIENDS TO ALLOW CALLING PROTECTED & PRIVATE FUNCTIONS
    friend class NETVSendThread;
    friend class NETVRecvThread;

    /**
        This working thread is responsible of reading NETV messages from the device
    */
    class NETVRecvThread : public QThread
    {
        public:

        ///Constructor taking the NETVInterfaceHandler pointer
        NETVRecvThread(NETVInterfaceHandler *handler);
        
        /** 
            Run will start receving from the NETV device.
            recvThreadFunction() is blocking (1 sec timeout)
        */
        virtual void run ();
		
        /**
            Sleeping the thread for determined microseconds
            \param microseconds the number of us to sleep
        */
        void thread_usleep(unsigned long microseconds);
        
        private:

        ///NETVInterfaceHandler (parent)
        NETVInterfaceHandler *m_handler;
    };

    /**
        This working thread is responsible of writing NETV messages to the device
    */
    class NETVSendThread : public QThread
    {
        public:
        ///Constructor taking the NETVInterfaceHandler pointer
        NETVSendThread(NETVInterfaceHandler *handler);

        /** 
            Run will start sending to the NETV device.
            sendThreadFunction() is blocking (1 sec timeout)
        */
        virtual void run ();
		
        /**
            Sleeping the thread for determined microseconds
            \param microseconds the number of us to sleep
        */
        void thread_usleep(unsigned long microseconds);
        
        private:

        ///NETVInterfaceHandler (parent)
        NETVInterfaceHandler *m_handler;
    };

    /**
        Constructor
        \param device The \ref NETVDevice to use
        \param args The arguments to send to the device
        \param parent Any QObject as parent. NULL if no parent.
    */
    NETVInterfaceHandler(NETVDevice *device, const char* args, QObject *parent = NULL);
	
    ///Destructor, will terminate threads and close the NETV device
    virtual ~NETVInterfaceHandler();

    /**
        Get the running flags for threads
        \return bool True if the threads are running
    */
    bool isRunning();

    /**
        Register an observer. The observer will receive (ALL) incoming NETV messages.
        \param observer The observer will be added to the observers list.
    */
    void registerObserver(NETVMessageObserverIF *observer);

    /**
        Unregister an observer. The observer will stop receiving incoming NETV messages
        \param observer The observer will be removed from the observers list.
    */
    void unregisterObserver(NETVMessageObserverIF *observer);

    /**
        Push a NETV_MESSAGE in the message queue.
        \param msg The message to send
    */
    void pushNETVMessage(const NETV_MESSAGE &msg);

    /**
        Push a list of NETV_MESSAGE in the message queue.
        \param msgList The list containing the messages to send
    */
    void pushNETVMessages(const std::list<NETV_MESSAGE> &msgList);

    /**
        \return int The count of message received
    */
    int messageReceivedCounter();

    /**
        \return int The count of message sent
    */
    int messageSentCounter();

    /**
        \return the Device handle
    */
    NETVDevice* getDevice();

    protected slots:

    ///Will be called when the send thread ends
    void sendThreadFinished();

    ///Will be called when the receive thread ends
    void recvThreadFinished();


    protected:

    /**
        This function will send messages one at a time
        Will block on the semaphore until we have something to send.
        This function is called from the send worker thread
    */
    void sendThreadFunction();

    /**
        This function will receive NETV messages from the device.
        The function will make a blocking read.
        This function is called from the recv worker thread.
    */
    void recvThreadFunction();

    /**
        Notify observers that a new NETV message is available
    */
    void notifyCANObservers(const NETV_MESSAGE &msg);

    
    ///The abstract can device
    NETVDevice *m_device;
    ///The running flag is used for worker threads, they will iterate while it is true
    bool m_running;
    ///Message queue (for sending)
    std::list<NETV_MESSAGE> m_msgQueue;
    ///Mutex to protect the message queue
    QMutex m_listMutex;
    ///Mutex to protect the observer list
    QMutex m_observerMutex;
    ///Semaphore used for synchronizing the send thread with the send queue
    QSemaphore m_listSemaphore;
    ///List of message observers
    std::list<NETVMessageObserverIF*> m_observers;
    ///The send working thread
    NETVSendThread *m_NETVSendThread;
    ///The recv working thread
    NETVRecvThread *m_NETVRecvThread;
    ///Counter of message sent
    int m_messageSent;    
    ///Counter of message received
    int m_messageReceived;
};

#endif


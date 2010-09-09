/**
     Copyright (C) 2009-2010 IntRoLab
     http://introlab.gel.usherbrooke.ca
     Dominic Letourneau, ing. M.Sc.A.
     Dominic.Letourneau@USherbrooke.ca
 */

#ifndef _CAN_INTERFACE_HANDLER_H_
#define _CAN_INTERFACE_HANDLER_H_

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QSemaphore>
#include "CANDevice.h"
#include <QCoreApplication>
#include <QTimer>



/**
   This interface will allow us to receive CAN messages if we subscribe to 
   the service. Right now messages are not filtered, and we will receive
   all the CAN messages on the bus if we subscribe to the service. You must derive
   from this class to be able to receive messages.

    \author Dominic Letourneau
    \date 22/05/2008
*/
class CANMessageObserverIF
{
    public:
    ///notify function for new messages
    virtual void notifyCANMessage(const LABORIUS_MESSAGE &msg) = 0;
};


/**
    CANInterfaceHandler will handle CAN communication. This class is thread safe.
    A receive and a send thread will be created for communication.
    \author Dominic Letourneau
    \date 22/05/2008
*/
class CANInterfaceHandler : public QObject
{
    Q_OBJECT;

    public:


    ///WORKER THREADS ARE FRIENDS TO ALLOW CALLING PROTECTED & PRIVATE FUNCTIONS
    friend class CANSendThread;
    friend class CANRecvThread;

    /**
        This working thread is responsible of reading CAN messages from the device
    */
    class CANRecvThread : public QThread
    {
        public:

        ///Constructor taking the CANInterfaceHandler pointer
        CANRecvThread(CANInterfaceHandler *handler);
        
        /** 
            Run will start receving from the CAN device.
            recvThreadFunction() is blocking (1 sec timeout)
        */
        virtual void run ();
		
        /**
            Sleeping the thread for determined microseconds
            \param microseconds the number of us to sleep
        */
        void thread_usleep(unsigned long microseconds);
        
        private:

        ///CANInterfaceHandler (parent)
        CANInterfaceHandler *m_handler;
    };

    /**
        This working thread is responsible of writing CAN messages to the device
    */
    class CANSendThread : public QThread
    {
        public:
        ///Constructor taking the CANInterfaceHandler pointer
        CANSendThread(CANInterfaceHandler *handler);

        /** 
            Run will start sending to the CAN device.
            sendThreadFunction() is blocking (1 sec timeout)
        */
        virtual void run ();
		
        /**
            Sleeping the thread for determined microseconds
            \param microseconds the number of us to sleep
        */
        void thread_usleep(unsigned long microseconds);
        
        private:

        ///CANInterfaceHandler (parent)
        CANInterfaceHandler *m_handler;
    };

    /**
        Constructor
        \param device The \ref CANDevice to use
        \param args The arguments to send to the device
        \param parent Any QObject as parent. NULL if no parent.
    */
    CANInterfaceHandler(CANDevice *device, const char* args, QObject *parent = NULL);
	
    ///Destructor, will terminate threads and close the CAN device
    ~CANInterfaceHandler();

    /**
        Get the running flags for threads
        \return bool True if the threads are running
    */
    bool isRunning();

    /**
        Register an observer. The observer will receive (ALL) incoming CAN messages.
        \param observer The observer will be added to the observers list.
    */
    void registerObserver(CANMessageObserverIF *observer);

    /**
        Unregister an observer. The observer will stop receiving incoming CAN messages
        \param observer The observer will be removed from the observers list.
    */
    void unregisterObserver(CANMessageObserverIF *observer);

    /**
        Push a LABORIUS_MESSAGE in the message queue.
        \param msg The message to send
    */
    void pushCANMessage(const LABORIUS_MESSAGE &msg);

    /**
        Push a list of LABORIUS_MESSAGE in the message queue.
        \param msgList The list containing the messages to send
    */
    void pushCANMessages(const std::list<LABORIUS_MESSAGE> &msgList);

    /**
        \return int The count of message received
    */
    int messageReceivedCounter();

    /**
        \return int The count of message sent
    */
    int messageSentCounter();


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
        This function will receive CAN messages from the device.
        The function will make a blocking read.
        This function is called from the recv worker thread.
    */
    void recvThreadFunction();

    /**
        Notify observers that a new CAN message is available
    */
    void notifyCANObservers(const LABORIUS_MESSAGE &msg);

    
    ///The abstract can device
    CANDevice *m_device;
    ///The running flag is used for worker threads, they will iterate while it is true
    bool m_running;
    ///Message queue (for sending)
    std::list<LABORIUS_MESSAGE> m_msgQueue;
    ///Mutex to protect the message queue
    QMutex m_listMutex;
    ///Mutex to protect the observer list
    QMutex m_observerMutex;
    ///Semaphore used for synchronizing the send thread with the send queue
    QSemaphore m_listSemaphore;
    ///List of message observers
    std::list<CANMessageObserverIF*> m_observers;
    ///The send working thread
    CANSendThread *m_canSendThread;
    ///The recv working thread
    CANRecvThread *m_canRecvThread;
    ///Counter of message sent
    int m_messageSent;    
    ///Counter of message received
    int m_messageReceived;
};

#endif


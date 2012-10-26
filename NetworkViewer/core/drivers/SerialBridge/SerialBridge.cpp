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

#include "SerialBridge.h"
#include <QDebug>
#include <QCoreApplication>
#include <QDateTime>
#include "SerialPortSelectionDialog.h"

static bool SERIAL_BRIDGE_DEVICE_INIT = NETVDevice::registerDeviceFactory("SerialBridge",new NETVDevice::DeviceFactory<SerialBridge>("COM4;1000000","SerialPort;speed. To be used with PIC32 Module."));


//Template specialization for configure
template<>
QString NETVDevice::DeviceFactory<SerialBridge>::configure()
{
    SerialPortSelectionDialog myDialog;
    if (myDialog.exec())
    {
        //Dialog Accepted
        return QString(myDialog.getSerialPortString()+ ";"
                       + myDialog.getBaudRateString());
    }
    else
    {
        //Dialog Rejected
        return QString("");
    }
}


SerialBridge::SerialBridge(const char* args)
    : m_serialPort(NULL)
{

    serialBytesIn = 0;
    serialBytesOut = 0;
    initialize(args);
}

SerialBridge::~SerialBridge()
{
    if (m_serialPort)
    {
        delete m_serialPort;
        m_serialPort = NULL;
    }
}

NETVDevice::State SerialBridge::initialize(const char* args)
{

    QStringList config = QString(args).split(";");
    Q_ASSERT(config.size() == 2);

    //Port configuration
    PortSettings s;
    s.BaudRate = (BaudRateType) config[1].toInt();
    s.DataBits =  DATA_8;
    s.FlowControl = FLOW_OFF;
    s.Parity = PAR_NONE;
    s.StopBits = STOP_1;
    s.Timeout_Millisec = 1;
    m_serialPort = new QextSerialPort(config[0], s);

    if (!m_serialPort->open(QIODevice::ReadWrite))
    {
        qDebug("SerialBridge::initialize() : Cannot open serial port : %s",args);
        delete m_serialPort;
        m_serialPort = NULL;
        return NETVDevice::NETVDEVICE_FAIL;
    }
    else
    {
        //connect signals
        qDebug("SerialBridge::initialize() : Opening config : %s, baudrate : %i",args,s.BaudRate);

        connect(m_serialPort,SIGNAL(readyRead()),this,SLOT(serialReadyRead()));
        connect(m_serialPort,SIGNAL(bytesWritten(qint64)),this,SLOT(serialBytesWritten(qint64)));

        m_testTimer = new QTimer(this);
        connect(m_testTimer,SIGNAL(timeout()),this,SLOT(testTimer()));
        m_testTimer->start(1000);


        return NETVDevice::NETVDEVICE_OK;
    }
}

NETVDevice::State SerialBridge::sendMessage(NETV_MESSAGE &message)
{
    //BE CAREFUL THIS FUNCTION RUNS IN ANOTHER THREAD
    if (m_serialPort)
    {
	//We need to post the event so the message is processed in the right thread
        QCoreApplication::postEvent(this,new SerialBridgeSendEvent(message));
	
        return NETVDevice::NETVDEVICE_OK;
    }
    else
    {
        return NETVDevice::NETVDEVICE_FAIL;
    }
}

NETVDevice::State SerialBridge::recvMessage(NETV_MESSAGE &message)
{
    //BE CAREFUL THIS FUNCTION RUNS IN ANOTHER THREAD
    NETVDevice::State returnState = NETVDevice::NETVDEVICE_FAIL;

    bool acquire = m_recvSemaphore.tryAcquire(1,10); //try acquire for 10ms

    if ( m_serialPort && acquire)
    {
        //not yet received messages
        returnState = NETVDevice::NETVDEVICE_UNDERFLOW;

	m_recvQueueMutex.lock();

	int recvSize = m_recvQueue.size();
	
	if (recvSize > 0)
	{
            CANRxMessageBuffer msg;
            msg = m_recvQueue.front();
            m_recvQueue.pop_front();

            //Change the return state
            returnState = NETVDevice::NETVDEVICE_OK;

            unsigned int SID = (msg.messageWord[0]) & 0x000007FF;
            unsigned int EID = (msg.messageWord[1] >> 10)  & 0x0003FFFF;


            //Here convert the message buffer to a NETV_CAN message
            message.msg_priority = (SID >> 8) & 0x07;
            message.msg_type = (SID) & 0xFF;
            message.msg_boot = (EID >> 16) & 0x03 ;
            message.msg_cmd = (EID >> 8) & 0xFF;
            message.msg_dest = (EID) & 0xFF;
            message.msg_data_length = msg.msgEID.DLC;

            //copy data
            for (int i = 0; i < 8; i++)
            {
                message.msg_data[i] = msg.data[i];
            }

            message.msg_remote = msg.msgEID.RTR;
            message.msg_filter_hit = msg.msgSID.IFACE;
            message.msg_timestamp = QDateTime::currentDateTime().toMSecsSinceEpoch();

            //NETVDevice::printMessage(message);
	}
	
	m_recvQueueMutex.unlock();
    }//if serialport


    return returnState;
}

bool SerialBridge::newMessageReady()
{
    bool ready = false;
    m_recvQueueMutex.lock();
    ready = !m_recvQueue.empty();
    m_recvQueueMutex.unlock();
    return ready;
}

void SerialBridge::serialReadyRead()
{
    if (m_serialPort)
    {
        //Reading N complete messages
        int available = m_serialPort->bytesAvailable() / sizeof(CANRxMessageBuffer);

        serialBytesIn += available * sizeof(CANRxMessageBuffer);

        QByteArray array = m_serialPort->read(sizeof(CANRxMessageBuffer) * available);

        if (available <= 0)
        {
            //qDebug("SerialBridge::serialReadyRead() -> not enough bytes received, returning");
            return;
        }


        //qDebug("SerialBridge::serialReadyRead() available messages : %i",available);
	//Let's verify if we have received messages that are well aligned with the size of the
	//CANRxMessageBuffer data structure size
	Q_ASSERT(array.size() % sizeof(CANRxMessageBuffer) == 0);
	
	for (int i = 0; i < array.size(); i+= sizeof(CANRxMessageBuffer))
	{

            CANRxMessageBuffer msg;
            memcpy((char*) &msg.messageWord[0], &array.data()[i],sizeof(CANRxMessageBuffer));

            //qDebug("RECV FROM IFACE %i",msg.msgSID.IFACE);

            m_recvQueueMutex.lock();
            m_recvQueue.push_back(msg);
            m_recvQueueMutex.unlock();

            m_recvSemaphore.release(1);

            //debugging
            //qDebug("CANRxMessageBuffer word 0 : 0x%8x",msg.messageWord[0]);
            //qDebug("CANRxMessageBuffer word 1 : 0x%8x",msg.messageWord[1]);
            //qDebug("CANRxMessageBuffer word 2 : 0x%8x",msg.messageWord[2]);
            //qDebug("CANRxMessageBuffer word 3 : 0x%8x",msg.messageWord[3]);
            //qDebug("CANRxMessageBuffer DATA LENGTH : %i",msg.msgEID.DLC);
	}
	
	//qDebug("MsgCnt : %i",msgCnt);
    }//if m_serialPort
}

void SerialBridge::serialBytesWritten(qint64 nbBytes)
{
    //qDebug("SerialBridge::serialBytesWritten = %i",nbBytes);
}

bool SerialBridge::event(QEvent *event)
{

    if(event->type() == QEvent::User)
    {

        SerialBridgeSendEvent *msgEvent = dynamic_cast<SerialBridgeSendEvent*>(event);

        if (msgEvent && m_serialPort)
        {
            //We have a message to send to the socket...
            CANTxMessageBuffer CANBuffer;

            memset((char*) &CANBuffer,0,sizeof(CANTxMessageBuffer));


            unsigned int ID = 0;

            //priority
            ID |= (((unsigned int)msgEvent->m_message.msg_priority << 26) & 0x1C000000);

            //type
            ID |= (((unsigned int)msgEvent->m_message.msg_type << 18) & 0x03FC0000);

            //boot
            ID |= (((unsigned int)msgEvent->m_message.msg_boot << 16) & 0x00030000);

            //cmd
            ID |= (((unsigned int)msgEvent->m_message.msg_cmd << 8) & 0x0000FF00);

            //dest
            ID |= (((unsigned int)msgEvent->m_message.msg_dest) & 0x000000FF);


            CANBuffer.msgSID.SID = (ID >> 18);
            CANBuffer.msgEID.EID = (ID & 0x0003FFFF);

            //Set the interface (default to 1)
            CANBuffer.msgSID.IFACE = ETHERNET_BRIDGE_CAN1;

            //Set extended message
            CANBuffer.msgEID.IDE = 1;

            //Those bits should always be cleared
            CANBuffer.msgEID.SRR = 0;
            CANBuffer.msgEID.RB0 = 0;
            CANBuffer.msgEID.RB1 = 0;

            //Set RTR
            CANBuffer.msgEID.RTR = msgEvent->m_message.msg_remote;

            //copy data length
            CANBuffer.msgEID.DLC = msgEvent->m_message.msg_data_length;

            //copy data
            for (int i = 0; i<8; i++)
            {
                CANBuffer.data[i] = msgEvent->m_message.msg_data[i];
            }


            serialBytesOut += sizeof(CANTxMessageBuffer);

            //Write to socket
            m_serialPort->write((char*) &CANBuffer, sizeof(CANTxMessageBuffer));

            //Make sure the socket is flushed...
            m_serialPort->flush();

            return true;
        }
    }

    return QObject::event(event);
}

 void SerialBridge::testTimer()
 {
    qDebug("IN: %li OUT: %li",serialBytesIn,serialBytesOut);

    serialBytesIn = 0;
    serialBytesOut = 0;
 }

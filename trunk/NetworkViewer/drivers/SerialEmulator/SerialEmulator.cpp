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

#include "SerialEmulator.h"
#include <QDebug>
#include <QCoreApplication>
#include <QDialog>

#include <QList>
#include <QTextStream>
#include <QTextEdit>
#include <QVBoxLayout>
#include "SerialEmulatorConfigure.h"


static bool SERIAL_BRIDGE_DEVICE_INIT = NETVDevice::registerDeviceFactory("SerialEmulator",new NETVDevice::DeviceFactory<SerialEmulator>("COM4;115200;0;5","SerialPort;speed;debug(optional);timeDelay(optional)"));


//Template specialization for configure
template<>
QString NETVDevice::DeviceFactory<SerialEmulator>::configure()
{
    SerialEmulatorConfigure myDialog;
    myDialog.exec();
    return QString(myDialog.getSerialPortString()+ ";" + myDialog.getBaudRateString());
}

SerialEmulator::SerialEmulator(const char* args)
    : m_serialPort(NULL), m_pollTimer(NULL), m_debug(false), m_timeDelay(5)
{

    serialBytesIn = 0;
    serialBytesOut = 0;
    serialBytesFlushed = 0;
    initialize(args);
}

SerialEmulator::~SerialEmulator()
{
    qDebug("SerialEmulator::~SerialEmulator()");
    if (m_serialPort)
    {
        delete m_serialPort;
    }
}

NETVDevice::State SerialEmulator::initialize(const char* args)
{

    QStringList config = QString(args).split(";");
    Q_ASSERT(config.size() >= 2);

    //Port configuration
    PortSettings s;
    s.BaudRate = (BaudRateType) config[1].toInt();
    s.DataBits =  DATA_8;
    s.FlowControl = FLOW_OFF;
    s.Parity = PAR_NONE;
    s.StopBits = STOP_1;
    s.Timeout_Millisec = 1;
    m_serialPort = new QextSerialPort(config[0], s);


    if (config.size() == 3)
    {
        m_debug = (bool) config[2].toInt();
    }

    if (config.size() == 4)
    {
        m_timeDelay = config[3].toInt();
    }

    if (!m_serialPort->open(QIODevice::ReadWrite))
    {
        qDebug("SerialEmulator::initialize() : Cannot open serial port : %s",args);
        delete m_serialPort;
        m_serialPort = NULL;
        return NETVDevice::NETVDEVICE_FAIL;
    }
    else
    {
        m_openTime = QTime::currentTime();
        m_openTime.start();

        //connect signals
        qDebug("SerialEmulator::initialize() : Opening config : %s, baudrate : %i",args,s.BaudRate);
        connect(m_serialPort,SIGNAL(readyRead()),this,SLOT(serialReadyRead()));
        connect(m_serialPort,SIGNAL(bytesWritten(qint64)),this,SLOT(serialBytesWritten(qint64)));
        m_testTimer = new QTimer(this);
        connect(m_testTimer,SIGNAL(timeout()),this,SLOT(testTimer()));
        m_testTimer->start(1000);
        return NETVDevice::NETVDEVICE_OK;
    }
}

NETVDevice::State SerialEmulator::sendMessage(NETV_MESSAGE &message)
{
    //BE CAREFUL THIS FUNCTION RUNS IN ANOTHER THREAD
    if (m_serialPort)
    {

        if (m_openTime.elapsed() / 1000 > m_timeDelay)
        {
            //We need to post the event so the message is processed in the right thread
            QCoreApplication::postEvent(this,new SerialEmulatorSendEvent(message));
        }
        else
        {
            qDebug("Discarding message elapsed : %i minimum delay : %i",m_openTime.elapsed() / 1000, m_timeDelay);
        }

        return NETVDevice::NETVDEVICE_OK;
    }
    else
    {
        return NETVDevice::NETVDEVICE_FAIL;
    }
}

NETVDevice::State SerialEmulator::recvMessage(NETV_MESSAGE &message)
{

    //qDebug("NETVDevice::State SerialEmulator::recvMessage(NETV_MESSAGE &message)");
    //BE CAREFUL THIS FUNCTION RUNS IN ANOTHER THREAD
    NETVDevice::State returnState = NETVDevice::NETVDEVICE_FAIL;

    bool acquire = m_recvSemaphore.tryAcquire(1,10); //try acquire for 10ms


    if ( m_serialPort && acquire)
    {
        //not yet received messages
        returnState = NETVDevice::NETVDEVICE_OVERFLOW;

	m_recvQueueMutex.lock();

	int recvSize = m_recvQueue.size();
	
	if (recvSize > 0)
	{
            CANSerialMessage buf;
            buf = m_recvQueue.front();
            m_recvQueue.pop_front();

            //Change the return state
            returnState = NETVDevice::NETVDEVICE_OK;

            //Convert data structures
            message.msg_priority = (buf.pri_boot_rtr >> 5) & 0x07;
            message.msg_type = buf.type;
            message.msg_boot = (buf.pri_boot_rtr >> 3) & 0x03;
            message.msg_cmd = buf.cmd;
            message.msg_dest = buf.dest;
            message.msg_remote = (buf.pri_boot_rtr & 0x01);
            message.msg_data_length = (buf.data_length_iface >> 4) & 0x0F;

            //copy data
            for(unsigned i = 0; i < 8; i++)
            {
                message.msg_data[i] = buf.data[i];
            }


            //NETVDevice::printMessage(message);

	}
	
	m_recvQueueMutex.unlock();
    }//if serialport


    return returnState;
}

bool SerialEmulator::newMessageReady()
{
    bool ready = false;
    m_recvQueueMutex.lock();
    ready = !m_recvQueue.empty();
    m_recvQueueMutex.unlock();
    return ready;
}

void SerialEmulator::serialReadyRead()
{


    if (m_serialPort)
    {

        //Sync with START_BYTE
        while ((m_serialPort->bytesAvailable() > 0) &&
               ((unsigned char) m_serialPort->peek(1).at(0) != (unsigned char) START_BYTE))
        {
            if ((unsigned char) m_serialPort->peek(1).at(0) == (unsigned char)(START_BYTE))
            {
                break;
            }
            else
            {
                //Flushing byte
                unsigned char byte = m_serialPort->read(1).at(0);
                qDebug("SerialEmulator::serialReadyRead() - Flushing byte : %2.2x",(int) byte);
                serialBytesFlushed++;
            }
        }



        //Reading N complete messages
        int available = m_serialPort->bytesAvailable() / sizeof(CANSerialMessage);

        serialBytesIn += available * sizeof(CANSerialMessage);

        QByteArray array = m_serialPort->read(sizeof(CANSerialMessage) * available);

        if (available <= 0)
        {
            //qDebug("SerialEmulator::serialReadyRead() -> not enough bytes received, returning");
            return;
        }


        Q_ASSERT(array.size() % sizeof(CANSerialMessage) == 0);
	
        for (int i = 0; i < array.size(); i+= sizeof(CANSerialMessage))
	{

            CANSerialMessage msg;
            memcpy((char*) &msg.messageBytes[0], &array.data()[i],sizeof(CANSerialMessage));

            if (m_debug)
            {
                qDebug("++++> Recv Message Bytes : %2.2x %2.2x %2.2x %2.2x %2.2x %2.2x %2.2x %2.2x %2.2x %2.2x %2.2x %2.2x %2.2x %2.2x %2.2x",

                        (int) msg.messageBytes[0],
                        (int) msg.messageBytes[1],
                        (int) msg.messageBytes[2],
                        (int) msg.messageBytes[3],
                        (int) msg.messageBytes[4],
                        (int) msg.messageBytes[5],
                        (int) msg.messageBytes[6],
                        (int) msg.messageBytes[7],
                        (int) msg.messageBytes[8],
                        (int) msg.messageBytes[9],
                        (int) msg.messageBytes[10],
                        (int) msg.messageBytes[11],
                        (int) msg.messageBytes[12],
                        (int) msg.messageBytes[13],
                        (int) msg.messageBytes[14]
                       );

            }

            //Look for START_BYTE and VALIDATE CHECKSUM
            if ((msg.start_byte == START_BYTE) && (msg.checksum == serial_calculate_checksum(&msg)))
            {
                m_recvQueueMutex.lock();
                m_recvQueue.push_back(msg);
                m_recvQueueMutex.unlock();

                //release one message
                m_recvSemaphore.release(1);

            }
            else
            {
                qDebug("SerialEmulator::serialReadyRead() - Checksum error found : %i, calculated: %i, start_byte : %i",(int)msg.checksum,(int) serial_calculate_checksum(&msg),(int) msg.start_byte);
                qDebug("Checksum Message Bytes : %2.2x %2.2x %2.2x %2.2x %2.2x %2.2x %2.2x %2.2x %2.2x %2.2x %2.2x %2.2x %2.2x %2.2x %2.2x",

                       (int) msg.messageBytes[0],
                       (int) msg.messageBytes[1],
                       (int) msg.messageBytes[2],
                       (int) msg.messageBytes[3],
                       (int) msg.messageBytes[4],
                       (int) msg.messageBytes[5],
                       (int) msg.messageBytes[6],
                       (int) msg.messageBytes[7],
                       (int) msg.messageBytes[8],
                       (int) msg.messageBytes[9],
                       (int) msg.messageBytes[10],
                       (int) msg.messageBytes[11],
                       (int) msg.messageBytes[12],
                       (int) msg.messageBytes[13],
                       (int) msg.messageBytes[14]
                       );
            }
	}
	
	//qDebug("MsgCnt : %i",msgCnt);
    }//if m_serialPort

}

void SerialEmulator::serialBytesWritten(qint64 nbBytes)
{
    //qDebug("SerialEmulator::serialBytesWritten = %i",nbBytes);
}

bool SerialEmulator::event(QEvent *event)
{

    if(event->type() == QEvent::User)
    {

        SerialEmulatorSendEvent *msgEvent = dynamic_cast<SerialEmulatorSendEvent*>(event);

        if (msgEvent && m_serialPort)
        {
            //We have a message to send to the socket...
            CANSerialMessage buf;


            memset((char*) &buf,0,sizeof(CANSerialMessage));


            //Set fields
            buf.start_byte = START_BYTE;
            buf.pri_boot_rtr = (msgEvent->m_message.msg_priority << 5) | (msgEvent->m_message.msg_boot << 3) | (msgEvent->m_message.msg_remote);
            buf.type = msgEvent->m_message.msg_type;
            buf.cmd = msgEvent->m_message.msg_cmd;
            buf.dest = msgEvent->m_message.msg_dest;
            buf.data_length_iface = (msgEvent->m_message.msg_data_length <<4);

            //copy data
            for (unsigned int i = 0 ; i < 8; i++)
            {
                buf.data[i] = msgEvent->m_message.msg_data[i];
            }

            //calculate checksum
            buf.checksum = serial_calculate_checksum(&buf);


            serialBytesOut += sizeof(CANSerialMessage);

            //Write to serial port
            m_serialPort->write((char*) &buf.messageBytes[0], sizeof(CANSerialMessage));

            //Make sure the socket is flushed...
            m_serialPort->flush();

            if (m_debug)
            {
                qDebug("----> Send Message Bytes : %2.2x %2.2x %2.2x %2.2x %2.2x %2.2x %2.2x %2.2x %2.2x %2.2x %2.2x %2.2x %2.2x %2.2x %2.2x",

                        (int) buf.messageBytes[0],
                        (int) buf.messageBytes[1],
                        (int) buf.messageBytes[2],
                        (int) buf.messageBytes[3],
                        (int) buf.messageBytes[4],
                        (int) buf.messageBytes[5],
                        (int) buf.messageBytes[6],
                        (int) buf.messageBytes[7],
                        (int) buf.messageBytes[8],
                        (int) buf.messageBytes[9],
                        (int) buf.messageBytes[10],
                        (int) buf.messageBytes[11],
                        (int) buf.messageBytes[12],
                        (int) buf.messageBytes[13],
                        (int) buf.messageBytes[14]
                       );
            }

            return true;
        }
    }

    return QObject::event(event);
}

void SerialEmulator::testTimer()
{
    if (m_debug)
    {
        qDebug("IN: %li OUT: %li FLUSHED: %li",serialBytesIn,serialBytesOut,serialBytesFlushed);
    }

    serialBytesIn = 0;
    serialBytesOut = 0;
}

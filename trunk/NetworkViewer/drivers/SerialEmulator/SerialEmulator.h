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

#ifndef _SERIAL_EMULATOR_H_
#define _SERIAL_EMULATOR_H_

#include <QTcpSocket>
#include "CANDevice.h"
#include <QSemaphore>
#include <QMutex>
#include <QList>
#include <QEvent>
#include <QTimer>
#include "qextserialport.h"
#include "qextserialenumerator.h"

#define START_BYTE 0xF2

typedef union
{
        struct {
                unsigned char start_byte;
                unsigned char pri_boot_rtr;
                unsigned char type;
                unsigned char cmd;
                unsigned char dest;
                unsigned char data_length_iface;
                unsigned char data[8];
                unsigned char checksum;
        };

        unsigned char messageBytes[15];
} CANSerialMessage;


inline unsigned char serial_calculate_checksum(const CANSerialMessage *message)
{
        unsigned char checksum = 0;
        int i = 0;

        //simple accumulation of bytes from start until checksum (excluded)
        for (i = 0; i < (sizeof(CANSerialMessage) - 1); i++)
        {
                checksum += message->messageBytes[i];
        }

        return checksum;

}

class SerialEmulator : public QObject, public CANDevice
{
	Q_OBJECT;

	public:


        class SerialEmulatorSendEvent : public QEvent
	{
		public:
		
                SerialEmulatorSendEvent(const LABORIUS_MESSAGE &message)
			:	QEvent(QEvent::User)
		{
			m_message = message;
		}
		
		LABORIUS_MESSAGE m_message;
	
	};

        SerialEmulator(const char* params);

	
	virtual CANDevice::State initialize(const char* args);


	  /** send a LABORIUS_MESSAGE
		   \param message The message to send
		   \return int The status after the message has been sent
	  */
	  virtual CANDevice::State sendMessage(LABORIUS_MESSAGE &message);

	  /** receive a LABORIUS_MESSAGE
		   \param message The message to receive (will be filled)
		   \return int The status after the message has been received
	  */
	  virtual CANDevice::State recvMessage(LABORIUS_MESSAGE &message);

	  /** Verify if a message is ready to receive
		   \return bool true if a message is ready to be received
	  */
	  virtual bool newMessageReady();
	
        protected slots:
	
        void serialReadyRead();
        void serialBytesWritten(qint64 nbBytes);
        void testTimer();
	
	
	protected:
	
	//Internal event processing...
	bool event(QEvent *event);
	
        QextSerialPort *m_serialPort;

	QTimer *m_pollTimer;


        QList<CANSerialMessage> m_sendQueue;
        QList<CANSerialMessage> m_recvQueue;
	QMutex m_sendQueueMutex;
	QMutex m_recvQueueMutex;
        
        long serialBytesIn;
        long serialBytesOut;
        long serialBytesFlushed;
        QTimer *m_testTimer;
};


#endif


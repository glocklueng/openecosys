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

#ifndef _SERIAL_BRIDGE_H_
#define _SERIAL_BRIDGE_H_

#include <QTcpSocket>
#include "CANDevice.h"
#include <QSemaphore>
#include <QMutex>
#include <QList>
#include <QEvent>
#include <QTimer>
#include "qextserialport.h"
#include "qextserialenumerator.h"


typedef enum {
	ETHERNET_BRIDGE_CAN1,
	ETHERNET_BRIDGE_CAN2
} CAN_IFACE;

typedef unsigned char  BYTE;	/* 8-bit unsigned  */
typedef unsigned int   UINT32;	/* other name for 32-bit integer */

// *****************************************************************************
/* CAN TX Message SID Word  

  Summary:
    Defines the structure of the SID word section of the TX message.  

  Description:
    This data structure represents the SID section of the CAN TX message. The data
    data structure is an element of the CANTxMessageBuffer data structure. 
*/

typedef	struct 
{
    // CAN TX Message Standard ID. This value should
    // be between 0x0 - 0x7FF.
	unsigned SID:11;
	
	//CAN interface number
	unsigned IFACE:5;
	
	unsigned :16;
}CAN_TX_MSG_SID;


// *****************************************************************************
/* CAN RX Message SID Word  

  Summary:
    Defines the structure of the SID word section of the RX message.  

  Description:
    This data structure represents the SID section of the CAN RX message. The data
    data structure is an element of the CANRxMessageBuffer data structure. 
*/

typedef	struct 
{
    // SID of the Received CAN Message. 
	unsigned SID:11;

	//CAN interface number
	unsigned IFACE:5;
	
    // Time stamp of the received message. This is
    // valid only if the Timestamping is enabled.
	unsigned CMSGTS:16;

}CAN_RX_MSG_SID;


// *****************************************************************************
/* CAN Message EID Word  

  Summary:
    Defines the structure of the EID word section of the TX and RX message.  

  Description:
    This data structure represents the EID section of the CAN TX and RX message. 
    The data structure is an element of the CANTxMessageBuffer and CANRxMessageBuffer
    data structure. 
*/
typedef struct 
{
    // Data Length Control. Specifies the size of the
    // data payload section of the CAN packet. Valid 
    // values are 0x0 - 0x8.
	unsigned DLC:4;

    // Reserved bit. Should be always 0.
	unsigned RB0:1;
	unsigned :3;

    // Reserved bit. Should be always 0.
	unsigned RB1:1;

    // Remote Transmit Request bit. Should be set for
    // RTR messages, clear otherwise.
	unsigned RTR:1;

    // CAN TX and RX Extended ID field. Valid values
    // are in range 0x0 - 0x3FFFF. 
	unsigned EID:18;

    // Identifier bit. If 0 means that message is SID.
    // If 1 means that message is EID type.
	unsigned IDE:1;

    // Susbtitute Remote request bit. This bit should
    // always be clear for an EID message. It is ignored
    // for an SID message.
	unsigned SRR:1;
	unsigned :2;

}CAN_MSG_EID;

// *****************************************************************************
/* CAN RX Message Buffer  

  Summary:
    Defines the structure of the CAN RX Message Buffer.  

  Description:
    This data structure represents the CAN RX Message Buffer. Received messages could
    be either full-receive messages or data-only messages. Full receive message contains
    the message header and data payload section.  For a full receive CAN RX channel, the
    caller should use the msgSID, msgEID and data members . A data-only message contains
    only a 8-byte data payload. While using this data structure with a Data-Only type of
    CAN RX channel, the caller should use the dataOnlyMsgData member of the structure and
    should read only 8 bytes of data.

     
*/

typedef union {

	struct
	{
        // This is SID portion of the CAN RX message.
		CAN_RX_MSG_SID msgSID;

        // This is EID portion of the CAN RX message
		CAN_MSG_EID msgEID;

        // This is the data payload section of the 
        // received message.
		BYTE data[7];
	};

    // This can be used if the message buffer is to 
    // be read from a Data-Only type of CAN RX Channel.
    BYTE dataOnlyMsgData[7];

    // This is CAN RX message organized as a set of 32 bit 
    // words.
	UINT32 messageWord[4];

}CANRxMessageBuffer;


// *****************************************************************************
/* CAN TX Message Buffer  

  Summary:
    Defines the structure of the CAN TX Message Buffer.  

  Description:
    This data structure represents the CAN TX Message Buffer. This data structure
    should be used for writing data to a CAN TX channel and transmitting data. A 
    pointer to this type of data structure is returned by the CANGetTxMessageBuffer()
    function.

    The data structure allows the TX message buffer to be accessed as fields of a CAN
    TX message and also as an array of 4 32-bit words. The latter allows for quick
    intialization of the message buffer.  
*/


typedef union {

	struct
	{
        // This is SID portion of the CAN TX message.
		CAN_TX_MSG_SID msgSID;

        // This is EID portion of the CAN TX message.
		CAN_MSG_EID msgEID;

        // This is the data portion of the CAN TX message.
		BYTE data[7];
	};

    // This is CAN TX message organized as a set of 32 bit 
    // words.
	UINT32 messageWord[4];

}CANTxMessageBuffer;


class SerialBridge : public QObject, public CANDevice
{
	Q_OBJECT;

	public:


        class SerialBridgeSendEvent : public QEvent
	{
		public:
		
                SerialBridgeSendEvent(const LABORIUS_MESSAGE &message)
			:	QEvent(QEvent::User)
		{
			m_message = message;
		}
		
		LABORIUS_MESSAGE m_message;
	
	};

        SerialBridge(const char* params);

        virtual ~SerialBridge();
	
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


	QList<CANTxMessageBuffer> m_sendQueue;
	QList<CANRxMessageBuffer> m_recvQueue;
	QMutex m_sendQueueMutex;
	QMutex m_recvQueueMutex;
        QSemaphore m_recvSemaphore;


        long serialBytesIn;
        long serialBytesOut;
        QTimer *m_testTimer;
};


#endif


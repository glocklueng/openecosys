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

#include "EthernetBridge.h"
#include <QDebug>
#include <QCoreApplication>

static bool ETHERNET_BRIDGE_DEVICE_INIT = NETVDevice::registerDeviceFactory("EthernetBridge",new NETVDevice::DeviceFactory<EthernetBridge>("192.168.1.95;6653","Host;port"));


EthernetBridge::EthernetBridge(const char* args)
    : QTcpSocket()
{
    initialize(args);
}

NETVDevice::State EthernetBridge::initialize(const char* args)
{
        QStringList config = QString(args).split(";");
        Q_ASSERT(config.size() == 2);
	connect(this,SIGNAL(connected()),this,SLOT(socketConnected()));
        connectToHost(config[0],config[1].toInt());
}

EthernetBridge::EthernetBridge(QString host, int port)
        :	QTcpSocket()
{
        connect(this,SIGNAL(connected()),this,SLOT(socketConnected()));
        connectToHost(host,port);
}


NETVDevice::State EthernetBridge::sendMessage(NETV_MESSAGE &message)
{
	//BE CAREFUL THIS FUNCTION RUNS IN ANOTHER THREAD
	
	//We need to post the event so the message is processed in the right thread
	QCoreApplication::postEvent(this,new EthernetBridgeSendEvent(message));
	
        return NETVDevice::NETVDEVICE_OK;
}

NETVDevice::State EthernetBridge::recvMessage(NETV_MESSAGE &message)
{
	//BE CAREFUL THIS FUNCTION RUNS IN ANOTHER THREAD
        NETVDevice::State returnState = NETVDevice::NETVDEVICE_UNDERFLOW;
	
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
		message.msg_dwTime = msg.msgSID.CMSGTS;
		
		//NETVDevice::printMessage(message);
		
		
	}
	
	m_recvQueueMutex.unlock();
	

	
	
	return returnState;
}

bool EthernetBridge::newMessageReady()
{


}



void EthernetBridge::socketConnected()
{
	qDebug("EthernetBridge::socketConnected()");
	
	connect(this,SIGNAL(readyRead()),this,SLOT(socketReadyRead()));
	
}

void EthernetBridge::socketReadyRead ()
{
	static int msgCnt = 0;

	QByteArray array = readAll();
	//qDebug("Received %i bytes TXBS:  %i RXBS: %i",array.size(), sizeof(CANTxMessageBuffer),sizeof(CANRxMessageBuffer));
	
	
	
	//Let's verify if we have received messages that are well aligned with the size of the
	//CANRxMessageBuffer data structure size
	Q_ASSERT(array.size() % sizeof(CANRxMessageBuffer) == 0);
	
	for (int i = 0; i < array.size(); i+= sizeof(CANRxMessageBuffer))
	{
		msgCnt++;
		CANRxMessageBuffer msg;
		memcpy((char*) &msg.messageWord[0], &array.data()[i],sizeof(CANRxMessageBuffer));
		
		//qDebug("RECV FROM IFACE %i",msg.msgSID.IFACE);
		
		m_recvQueueMutex.lock();
		m_recvQueue.push_back(msg);
		m_recvQueueMutex.unlock();
				
		//debugging
		//qDebug("CANRxMessageBuffer word 0 : 0x%8x",msg.messageWord[0]);
		//qDebug("CANRxMessageBuffer word 1 : 0x%8x",msg.messageWord[1]);
		//qDebug("CANRxMessageBuffer word 2 : 0x%8x",msg.messageWord[2]);
		//qDebug("CANRxMessageBuffer word 3 : 0x%8x",msg.messageWord[3]);
		
		//qDebug("CANRxMessageBuffer DATA LENGTH : %i",msg.msgEID.DLC);
	}
	
	//qDebug("MsgCnt : %i",msgCnt);
}

bool EthernetBridge::event(QEvent *event)
{
	if(event->type() == QEvent::User)
	{
		EthernetBridgeSendEvent *msgEvent = dynamic_cast<EthernetBridgeSendEvent*>(event);

		if (msgEvent)
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
			
			//Write to socket
			write((char*) &CANBuffer, sizeof(CANTxMessageBuffer));
			
			//Make sure the socket is flushed...
			flush();
			
			return true;
		}
	}

	return QTcpSocket::event(event);
}


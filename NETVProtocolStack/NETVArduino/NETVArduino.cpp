/**
 Copyright (C) 2009-2011 IntRoLab
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

#include "NETVArduino.h"

NETVArduino netvArduino;

NETVArduino::NETVArduino()
: m_projectID(0), m_moduleID(0), m_codeVersion(0), m_baudrate(0), m_table(NULL), m_size(0) 
{
	
}

void NETVArduino::setup(byte projectID, byte moduleID, byte codeVersion, unsigned long baudrate, byte* table, unsigned int size)

{
	m_projectID = projectID;
	m_moduleID = moduleID;
	m_codeVersion = codeVersion;
	m_baudrate = baudrate;
	m_table = table;
	m_size = size;


	if (m_table && m_size > 0)
	{	  	  
		//Open serial port
		Serial.begin(baudrate);
		  
		//Flush serial
		Serial.flush();

		//Empty serial data 
		while(Serial.available() > 0)
		{
			Serial.read();
		}
	}
}

bool toggle = false;

void NETVArduino::process_message(const NETVSerialMessage &message)
{
	if (message.type == NETV_TYPE_EVENTS)
	{
	
		//If it is an alive request, answer with our own address
		if (message.cmd == NETV_EVENTS_CMD_ALIVE)
		{
			for (unsigned int i= 0; i < sizeof(NETVSerialMessage); i++)
			{
				m_outgoingMessage.messageBytes[i] = message.messageBytes[i];
			}
			
			//Remove RTR
			m_outgoingMessage.pri_boot_rtr &= 0xFE;
			
			//Set dest since this message was sent to dest=0xFF (BROADCAST)
			m_outgoingMessage.dest = m_moduleID;
			
			
			//Fill data
			m_outgoingMessage.data[0] = NETV_NORMAL_MODE_ID;  //module state
			m_outgoingMessage.data[1] = m_projectID; //project_id
			m_outgoingMessage.data[2] = m_moduleID;//module_id
			m_outgoingMessage.data[3] = m_codeVersion;  //code version
			m_outgoingMessage.data[4] = 2;  //table version
			m_outgoingMessage.data[5] = 10; //boot delay
			m_outgoingMessage.data[6] = 0x12;  //device id
			m_outgoingMessage.data[7] = 0x34;  //device id
			
			//Recalculate checksum
			m_outgoingMessage.checksum = serial_calculate_checksum(m_outgoingMessage);
			
			
			//Send to serial port
			for (unsigned int i = 0; i < sizeof(NETVSerialMessage); i++)
			{
				Serial.write(m_outgoingMessage.messageBytes[i]);
			}
			
			//Flush serial
			Serial.flush();

		} //If it is an alive request

	}
	else if(message.type == NETV_TYPE_REQUEST_DATA && m_table != NULL)
	{
		if (message.dest == m_moduleID)
		{
			unsigned char eeprom_ram = (message.pri_boot_rtr >> 4) & 0x01;
			unsigned char read_write = (message.pri_boot_rtr >> 3) & 0x01;
			unsigned char length = (message.data_length_iface >> 4) & 0x0F;
			
			//Make sure we don't overflow
			if (message.cmd + length <= m_size)
			{
				//READING
				if (read_write == NETV_REQUEST_READ)
				{
					
					for (unsigned int i= 0; i < sizeof(NETVSerialMessage); i++)
					{
						m_outgoingMessage.messageBytes[i] = message.messageBytes[i];
					}
					
					//Remove RTR
					m_outgoingMessage.pri_boot_rtr &= 0xFE;
								
					//Copy data
					cli();
					for (unsigned int i = message.cmd; i < message.cmd + length; i++)
					{
						m_outgoingMessage.data[i - message.cmd] = m_table[i];
					}
					sei();
					//Recalculate checksum
					m_outgoingMessage.checksum = serial_calculate_checksum(m_outgoingMessage);
					
					
					//Send to serial port
					for (unsigned int i = 0; i < sizeof(NETVSerialMessage); i++)
					{
						Serial.write(m_outgoingMessage.messageBytes[i]);
					}
					
					//Flush serial
					Serial.flush();
					
				}
				else if (read_write == NETV_REQUEST_WRITE)
				{
					//WRITING
					cli();
					for (unsigned int i = message.cmd; i < message.cmd + length; i++)
					{
						m_table[i] = message.data[i - message.cmd];
					}
					sei();
				}
			}			
		} //Message intended for us?
	}	
}

void NETVArduino::transceiver()
{

	//Look for START_BYTE
	while (Serial.available() >= sizeof(NETVSerialMessage))
	{
		
		byte value = Serial.read();
				
		if (value == START_BYTE)
		{
			m_incomingMessage.messageBytes[0] = START_BYTE;
			
			//We have a starting message, let's read the rest of the data
		    for (unsigned int i = 1; i < sizeof(NETVSerialMessage); i++)
			{
				m_incomingMessage.messageBytes[i] = Serial.read();
			}
			
			//Verify the checksum			
			if (serial_calculate_checksum(m_incomingMessage) == m_incomingMessage.checksum)
			{
				//Process the message
				process_message(m_incomingMessage);					
			}	
			else
			{
				//TODO
				//Some kind of synchronisation here might be required
				if (Serial.available() > 0)
				{
					//offset one byte until the checksum is ok...					
					Serial.read();
				}
			}
		}	
	}
}


void NETVArduino::mapMemory(byte* table, unsigned int size)
{
	if (table && size > 0)
	{
       m_table = table;
       m_size = size;	  
    }
}


unsigned char NETVArduino::serial_calculate_checksum(const NETVSerialMessage &message)
{
	unsigned char checksum = 0;
	int i = 0;
		
	//simple accumulation of bytes from start until checksum (excluded)
	for (i = 0; i < (sizeof(NETVSerialMessage) - 1); i++)
	{
		checksum += message.messageBytes[i];
	}
		
	return checksum;
		
}

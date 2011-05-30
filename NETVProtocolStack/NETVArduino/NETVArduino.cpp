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
: m_table(NULL), m_size(0)
{
	
}

void NETVArduino::setup(byte* table, unsigned int size)
{
  if (table && size > 0)
  {
       m_table = table;
       m_size = size;
	  	  
	  //Open serial port
	  Serial.begin(115200);
	  
	  state = LOW;
	  
	  pinMode(77, OUTPUT);
	  digitalWrite(77, state);
	  
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
				digitalWrite(77, state);	
				
				if (state == LOW)
				{
					state = HIGH;
				}
				else 
				{
					state = LOW;
				}

				//Answer the request
				
			}
		
		}
		
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
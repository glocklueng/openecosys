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

#ifndef NETVArduino_h
#define NETVArduino_h

#include "WProgram.h"
#include "NETV_define.h"


/**
	Serial Message
 */
typedef union
{
	struct {
	byte start_byte;
	byte pri_boot_rtr;
	byte type;
	byte cmd;
	byte dest;
	byte data_length_iface;
	byte data[8];
	byte checksum;
	};
		
	byte messageBytes[15];
} NETVSerialMessage;

//The message start byte
#define START_BYTE 0xF2


class NETVArduino
{
	
public:
	
  NETVArduino();
  
/**
			m_outgoingMessage.data[0] = 0;  //module state
			m_outgoingMessage.data[1] = 50; //project_id
			m_outgoingMessage.data[2] = 200;//module_id
			m_outgoingMessage.data[3] = 1;  //code version
			m_outgoingMessage.data[4] = 2;  //table version
			m_outgoingMessage.data[5] = 10; //boot delay
			m_outgoingMessage.data[6] = 0;  //device id
			m_outgoingMessage.data[7] = 0;  //device id
*/  
	
  void setup(byte projectID, byte moduleID, byte codeVersion, unsigned long baudrate=115200, byte* table=0, unsigned int size=0);
  
  void mapMemory(byte* table, unsigned int size);
  
  void transceiver();  

private:

  byte m_projectID;
  byte m_moduleID;
  byte m_codeVersion;
  byte m_baudrate;
  byte* m_table;
  unsigned int m_size;
  
  
  //Buffers	
  NETVSerialMessage m_incomingMessage; 
  NETVSerialMessage m_outgoingMessage;
  void process_message(const NETVSerialMessage &message);
  unsigned char serial_calculate_checksum(const NETVSerialMessage &message);
  unsigned char state;	
};

//External instance
extern NETVArduino netvArduino;

#endif

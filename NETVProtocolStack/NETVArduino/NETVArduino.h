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
	
  void setup(byte* table, unsigned int size);	
  
  void transceiver();
  

private:

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

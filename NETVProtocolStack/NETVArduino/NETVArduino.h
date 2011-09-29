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


//The message start byte
#define START_BYTE 0xF2

/**
	Serial Message data structure. This is made
	compatible with NetworkViewer by emulating
	CAN messages through serial bus.
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



/*
	This class will handle communication with the serial port and
	reading / writing to a shared memory.
*/
class NETVArduino
{
	
public:
	
  //Default Constructor	
  NETVArduino();
  	
	
  /**
	setup needs to be called before using NETVarduino. It will set internal variables and
	open the serial port.
	
	\param projectID The id of the project that will be used by NetworkViewer to automatically load its configuration (XML files)
	\param moduleID The module id can be comprised between 0 and 254, 255 is reserved for broadcasting
	\param baudrate The baudrate for serial communication, no parity, 1 stop bit. Defaults to 115200 baud
	\param table This is the shared memory pointer you want to use
	\param size This is the size in bytes of the shared memory	
  */  
  void setup(byte projectID, byte moduleID, byte codeVersion, unsigned long baudrate=115200, byte* table=0, unsigned int size=0);
  
  /**
	Set the memory that will be mapped (if you did not do it in the setup function)
	\param table the shared memory pointer
	\param size the size of the shared memory
  */
  void mapMemory(byte* table, unsigned int size);
  
  
  /**
	Call this function in your main loop. It will look at available bytes on the serial port
	and parse it to fit with NETVProtocolStack serial protocol. Read/Write requests are
	handled automatically in this function.
  */
  void transceiver();  
	
	
  /**
	Send a message to the serial port.
    \param msg The message to send. Checksum will be (re) calculated.
  */
  void send(const NETVSerialMessage &msg);

	
  /**
	Manual update of one variable. Can be used for periodic update. The variable must be contained in the
    memory map.
    \param var the pointer to the variable.
    \param size the size of the variable. Must not exceed 8 bytes.
	\return bool TRUE is success
  */
  bool updateVariable(byte *var, unsigned int size);
	

private:

  ///Our projectID
  byte m_projectID;
  ///Our moduleID
  byte m_moduleID;
  ///Our code version
  byte m_codeVersion;
  ///Used baudrate
  byte m_baudrate;
  ///Shared memory pointer
  byte* m_table;
  ///Shared memory size
  unsigned int m_size;
  ///Input buffer
  NETVSerialMessage m_incomingMessage; 
  ///Output buffer
  NETVSerialMessage m_outgoingMessage;
  
  ///Internal processing of serial messages. This function is called when the cheksum is valid
  void process_message(const NETVSerialMessage &message);
  ///Calculates the checksum of a message
  unsigned char serial_calculate_checksum(const NETVSerialMessage &message);
	
};

//External instance
extern NETVArduino netvArduino;

#endif

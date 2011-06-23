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

/**
  This is the memory we want to share on the network. 
  Every variable is read/write and can be changed with the NetworkViewer.
  For this demonstration, we will use those variables to store the state
  of the ADC conversion on the first 6 channels.
  
  The union means that the struct is mapped with the byte array
  NETV_SHARED_MEMORY_TABLE so that the address of 
  analog0 == NETV_SHARED_MEMORY_TABLE[0]
*/
typedef union 
{
  struct 
  {
    unsigned int analog0; //AD Conversion #0
    unsigned int analog1; //AD Conversion #1
    unsigned int analog2; //AD Conversion #2
    unsigned int analog3; //AD Conversion #3
    unsigned int analog4; //AD Conversion #4
    unsigned int analog5; //AD Conversion #5
  };
  
  //The same memory addressed with an array
  unsigned char NETV_SHARED_MEMORY_TABLE[];
  
} NETVSharedVariables;


//Let's declare the variables here. They will be global.
NETVSharedVariables variables;

void setup() { 

  //This will configure the NETVProtocolStack to work with our project.  
  netvArduino.setup(50, //Project ID, NetworkViewer will look at this ID for automatic loading of configuration (XML files)
              1, //Module ID (you can use anything from 0 to 254, 255 = broadcast address)      
              1, //The Code Version, this is useful if you want to track the version of the actual code
              115200, //The baud rate, recommanded = 115200
              variables.NETV_SHARED_MEMORY_TABLE, //The memory we would like to share with the NETVProtocolStack
              sizeof(NETVSharedVariables));  //The size of the memory mapped
}

void loop() 
{
  
        //This will process data on the serial port and read/write to the shared memory
	netvArduino.transceiver();

        //Update the state of the memory
	variables.analog0 = analogRead(A0);
	variables.analog1 = analogRead(A1); 
	variables.analog2 = analogRead(A2); 
	variables.analog3 = analogRead(A3); 
	variables.analog4 = analogRead(A4); 
	variables.analog5 = analogRead(A5); 
}

/*
The OpenEcoSys project / CANProtocolStack
Copyright (C) 2011  IntRoLab - Universite de Sherbrooke

Author(s)

Dominic Letourneau, ing., M.Sc.A.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "NETV16_Memory.h"
#include "bsp.h"
#include <libpic30.h>

int __attribute__ ((space(eedata))) eeData = 0x1234; // Variable located in EEPROM  //Used as the base address


unsigned int WriteMem(unsigned int offset, unsigned int* dataPtr, unsigned int size)
{
	while(size--)
	{
		ee_word_write(offset, (*dataPtr++));
		offset += 2;	//1 word
	}
	return 0;
}

unsigned long ReadMem(unsigned int offset)
{
	return ee_word_read(offset);
}


//Write a value at adress EepromBase (0x7FFE00) + Offset
void ee_word_write(unsigned int offset, int data)
{

/**
	unsigned int memory_case;

	// Set up NVMCON to erase one word of data EEPROM
	NVMCON = 0x4004;
	
	// Set up a pointer to the EEPROM location to be erased
	TBLPAG = __builtin_tblpage(&eeData); // Initialize EE Data page pointer
	memory_case = __builtin_tbloffset(&eeData) + offset; // Initialize lower word of address
	
	__builtin_tblwtl(memory_case, data); // Write EEPROM data to write latch
	
	asm volatile ("disi #5"); // Disable Interrupts For 5 Instructions
	__builtin_write_NVM(); // Issue Unlock Sequence & Start Write Cycle
*/
	_prog_addressT p;
	_init_prog_address(p, eeData);
	p += offset;
	_erase_eedata(p,_EE_WORD);	
	_wait_eedata();
	_write_eedata_word(p,data);	
	_wait_eedata();
}

//Read a value from adress EepromBase (0x7FFE00) + Offset
int ee_word_read(unsigned int offset)
{
	unsigned int memory_case;
	int data;

	// Set up a pointer to the EEPROM location to be erased
	TBLPAG = __builtin_tblpage(&eeData); // Initialize EE Data page pointer
	memory_case = __builtin_tbloffset(&eeData)+offset; // Initialize lower word of address
	data = __builtin_tblrdl(memory_case); // Write EEPROM data to write latch

	return data;
}

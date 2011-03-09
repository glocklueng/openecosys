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

#ifndef _CAN_MEMORY_H_
#define _CAN_MEMORY_H_

/*
	THIS FUNCTION IS IN ASSEMBLY IN THE FILE eeprom_flash.s
	This will read a memory address (on at a time)
	and return its value.

	\param addr_high Addres high word
	\param addr_low Address low word
	\return unsigned long data or program value
	Note :
	If you read flash memory, you will get a 32 bits value with 0x00HHMMLL (24 bits instruction)
	If you read eeprom/config/devid value, you will get a 16 bit value with 0x0000MMLL (16 bits data)
*/
unsigned long ReadMem(unsigned int addr_high, unsigned int addr_low);

/*
	THIS FUNCTION IS IN ASSEMBLY IN THE FILE eeprom_flash.s
	This will write to memory and return the write size.

	\param addr_high Addres high word
	\param addr_low Address low word
	\param dataPtr The pointer to data
	\param size The size of data to write (word size = 2 bytes)
	\return unsigned int the size of data effectively written

	Note :
	
	This function supports the following size (only!)

	1) Write to flash(page), size = 64 words = 32 instructions. 
	Make sure your address is aligned on pages. Pages are cleared before writing.

	2) Write to eeprom(word), size = 1, single word. Data is cleare before writing.


	3) Write to eeprom (page), size = 16, 16 words.
	Make sure your address is aligned on pages. Pages are cleared before writing.


	4) Write to config, size = 1, single world 
	--------->Not yet implemented for security reasons

*/
unsigned int WriteMem(unsigned int addr_high, unsigned int addr_low, unsigned int* dataPtr, unsigned int size);

//Built-in functions:
void ee_word_write(unsigned int offset, int data);
int ee_word_read(unsigned int offset);

#endif

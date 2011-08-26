


#ifndef _MEMORY_H_
#define _MEMORY_H_

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
extern unsigned long ReadMem(unsigned int addr_high, unsigned int addr_low);

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
extern unsigned int WriteMem(unsigned int addr_high, unsigned int addr_low, unsigned int* dataPtr, unsigned int size);

#endif

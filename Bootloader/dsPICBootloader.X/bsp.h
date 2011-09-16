#ifndef _BSP_H_
#define _BSP_H_

#ifdef _DSPIC30F5015_

#include <p30F5015.h>

#define FCY 20000000

//DEVICE MEMORY MAP INFORMATION
#define EEPROM_BASE_ADDRESS_LOW 0xFC00
#define EEPROM_BASE_ADDRESS_HIGH 0x007F
#define BOOTLOADER_ADDRESS 0X9000

#elif _DSPIC30F4011_

#include <p30F4011.h>

#define FCY 20000000

//DEVICE MEMORY MAP INFORMATION
#define EEPROM_BASE_ADDRESS_LOW 0xFC00
#define EEPROM_BASE_ADDRESS_HIGH 0x007F
#define BOOTLOADER_ADDRESS 0X6000

#endif






#endif


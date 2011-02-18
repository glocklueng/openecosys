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

#ifndef _CAN_DEVICE_H_
#define _CAN_DEVICE_H_

//--------------------------Device Configuration------------------------
//_FOSC(CSW_FSCM_OFF & HS2_PLL8);
//_FWDT(WDT_OFF);
//_FBORPOR(PBOR_OFF & BORV_42 & PWRT_64 & MCLR_EN);
//----------------------------------------------------------------------


//device file
#include <p30f5015.h>

#define FREQ_CYCLE    20000000	// xtal = 10Mhz; PLLx8 -> 20 MIPS		

//DEVICE MEMORY MAP INFORMATION
#define EEPROM_BASE_ADDRESS_LOW 0xFC00
#define EEPROM_BASE_ADDRESS_HIGH 0x007F



#endif

/*
The OpenEcoSys project / NETVProtocolStack
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

#include "bsp.h"
#include "NETV16_Common.h"

//--------------------------Device Configuration------------------------
_FOSC(CSW_FSCM_OFF & XT_PLL8);
_FWDT(WDT_OFF);
_FBORPOR(PBOR_OFF & BORV45 & PWRT_64 & MCLR_EN);
//----------------------------------------------------------------------

// Initial bootloader config in eeprom
// Variable located in EEPROM
int __attribute__ ((space(eedata))) g_bootData[8] = {BOOT_IDLE,
        0x0000, // PROJECT_ID
        0x0001, // MODULE_ID
        0x0001, // CODE_VERSION
        0x0002, // TABLE_VERSION
        0x0006, // BOOT_DELAY (SECONDS)
        0x0000, // DEVID LOW
        0x0000}; //DEVID HIGH


//Bootloader commands
enum {CMD_NOP,CMD_RESET,CMD_START_WRITE,CMD_END_WRITE};





void netv_proc_message(NETV_MESSAGE *msg)
{

}

//This code should be working with multiple interface...
int main()
{
    BootConfig *config = netv_get_boot_config();

    //Read actual configuration
    netv_read_boot_config(config);

    TRISBbits.TRISB13 = 0; //B13 = OUTPUT
    LATBbits.LATB13 = 0; //TURN ON LED

    while(1);

    return 0;
}


#if 0

#include "Device.h"
#include "CANDriverMinimal.h"
#include <timer.h>
#include "string.h"
//--------------------------Device Configuration------------------------
_FOSC(CSW_FSCM_OFF & HS2_PLL8);
_FWDT(WDT_OFF);
_FBORPOR(PBOR_OFF & BORV_45 & PWRT_64 & MCLR_EN);
//----------------------------------------------------------------------

/*
;* XXXXXXXXXXX 0 0 8 XXXXXXXX XXXXXX00 ADDRL ADDRH ADDRU RESVD CTLBT SPCMD CPDTL CPDTH
;* XXXXXXXXXXX 0 0 8 XXXXXXXX XXXXXX01 DATA0 DATA1 DATA2 DATA3 DATA4 DATA5 DATA6 DATA7

;* ADDRL - Bits 0 to 7 of the memory pointer.  
;* ADDRH - Bits 8 - 15 of the memory pointer.
;* ADDRU - Bits 16 - 23 of the memory pointer.
;* RESVD - Reserved for future use.
;* CTLBT - Control bits.
;* SPCMD - Special command.
;* CPDTL - Bits 0 - 7 of special command data.
;* CPDTH - Bits 8 - 15 of special command data.
;* DATAX - General data.

;* Control bits:
;* B0- MODE_WRT_UNLCK 	-	Set this to allow write and erase operations to memory.
;* B1- MODE_ERASE_ONLY 	- 	Set this to only erase Program Memory on a put command. Must 
;*						be on 64 byte boundary.
;* B2- MODE_AUTO_ERASE 	-	Set this to automatically erase Program Memory while writing data.
;* B3- MODE_AUTO_INC 	-	Set this to automatically increment the pointer after writing.
;* B4- MODE_ACK 		-	Set this to generate an acknowledge after a 'put' (PG Mode only)
;*
;* Special Commands:
;* CMD_NOP			0x00	Do nothing
;* CMD_RESET		0x01	Issue a soft reset
;* CMD_START_WRITE	0x02	Start write to memory
;* CMD_END_WRITE	0x03	End write to memory
;* 							

*/

#define CMD_NOP 0x00
#define CMD_RESET 0x01
#define CMD_START_WRITE 0x02
#define CMD_END_WRITE 0x03

//OLD DEFINES
//#define CAN_NORMAL_MODE_ID          0x05
//#define CAN_BOOT_MODE_ID            0x0A
//#define CAN_IDLE_MODE_ID            0x0B

enum {BOOT_IDLE=0x0A, BOOT_PROGRAM_FLASH=0x01, BOOT_PROGRAM_EEPROM=0x02, BOOT_NORMAL=0x05};

//initial config in eeprom
//MODULE_STATE=IDLE, PROJECT_ID = 0
//MODULE_ID = X, CODE_VERSION = 1, TABLE_VERSION = 2, BOOT_DELAY = 6, UNUSED = 0, UNUSED =0
unsigned int _EEDATA(2) g_bootData[8] = {0x000A,0x0000,0x0008,0x0001,0x0002,0x0006,0x0000,0x0000};


void readBootConfig(BootConfig *config)
{
	unsigned int devid;

	if (config)
	{
		config->module_state  = (unsigned char) ReadMem(EEPROM_BASE_ADDRESS_HIGH,EEPROM_BASE_ADDRESS_LOW);
		config->project_id  = (unsigned char) ReadMem(EEPROM_BASE_ADDRESS_HIGH,EEPROM_BASE_ADDRESS_LOW + 2);
		config->module_id  = (unsigned char) ReadMem(EEPROM_BASE_ADDRESS_HIGH,EEPROM_BASE_ADDRESS_LOW + 4);
		config->code_version  = (unsigned char) ReadMem(EEPROM_BASE_ADDRESS_HIGH,EEPROM_BASE_ADDRESS_LOW + 6);
		config->table_version = (unsigned char) ReadMem(EEPROM_BASE_ADDRESS_HIGH,EEPROM_BASE_ADDRESS_LOW + 8);
		config->boot_delay  = (unsigned char) ReadMem(EEPROM_BASE_ADDRESS_HIGH,EEPROM_BASE_ADDRESS_LOW + 10);

		//read devid
		devid = ReadMem(0xFF,0x0000);
		config->devid_low = devid & 0x00FF;
		config->devid_high = devid >> 8;

	}
}

void writeBootConfig(BootConfig* config)
{

	if (config)
	{

		unsigned int data[16]; //first page of data
		unsigned int addrlow = 0;

		//READING MEMORY
		for (addrlow = 0; addrlow < 32; addrlow += 2)
		{
			data[addrlow >> 1] = ReadMem(EEPROM_BASE_ADDRESS_HIGH,EEPROM_BASE_ADDRESS_LOW + addrlow);
		}

		data[0] = config->module_state;
		data[1] = config->project_id;
		data[2] = config->module_id;
		data[3] = config->code_version;
		data[4] = config->table_version;
		data[5] = config->boot_delay;
		//DEVID DOES NOT NEED TO BE WRITTEN!

		//WRITING BACK PAGE
		WriteMem(EEPROM_BASE_ADDRESS_HIGH,EEPROM_BASE_ADDRESS_LOW,data,16);

	}
}

int main()
{
	BootConfig moduleConfig;
	unsigned char timer1Count = 0;
	unsigned int flash_page[64] = {0};
	unsigned long base_address  = 0;
	//unsigned char boot_state = BOOT_IDLE;
	unsigned int page_index = 0;

	// GLOBAL VARIABLES TO BE USED IN ALL SOURCE FILES
	LABORIUS_MESSAGE g_rMessage = {0};    // all downloaded messages from PIC
                                          // are written here

   	LABORIUS_MASK mask_in[2];
   	LABORIUS_FILTER filter_in[6];
	unsigned char Can_Addr = 0xFE;

	//read boot configuration from eeprom
	readBootConfig(&moduleConfig);

	//Do not allow broadcast address
	if (moduleConfig.module_id != CAN_ADDRESS_BROADCAST)
	{
		Can_Addr = moduleConfig.module_id;
	}

	//Do not allow other modes than normal or idle
	if (moduleConfig.module_state != BOOT_NORMAL)
	{
		moduleConfig.module_state = BOOT_IDLE;
	}


/*
typedef struct tagRCONBITS {
        unsigned POR    :1;
        unsigned BOR    :1;
        unsigned IDLE   :1;
        unsigned SLEEP  :1;
        unsigned WDTO   :1;
        unsigned SWDTEN :1;
        unsigned SWR    :1;
        unsigned EXTR   :1;
        unsigned        :5;
        unsigned BGST   :1;
        unsigned IOPUWR :1;
        unsigned TRAPR  :1;
} RCONBITS;
*/
	//TODO What is the cause of the reset.
	//IF THE USER HAVE RESET MORE THAN 4 TIMES IN
	//2 SECONDS, PUT THE MODULE IN BOOT MODE
	if (RCONbits.POR)
	{
		//POWER ON RESET
		//RESET VARIABLE TO 0
	}
	else if (RCONbits.EXTR)
	{
		//EXTERNAL RESET
		//INCREMENT RESET VARIABLE
		//IF IN NORMAL MODE
	}


	//TODO
	//Set all ports to inputs
	TRISGbits.TRISG2 = 0; //G2 = output
	PORTGbits.RG2 = 1; //LED = OFF

	//setup default filters and masks
	memset(mask_in,0xFF,sizeof(LABORIUS_MASK) * 2);
	memset(filter_in,0xFF,sizeof(LABORIUS_FILTER) * 6);
	
	//we will be using mask 0 with filter 0 and 1
 	// init mask 0
	mask_in[0].mask_priority = 0;
    mask_in[0].mask_type = 0xFF;
    mask_in[0].mask_cmd = 0;
    mask_in[0].mask_dest = 0xFF;

    // init filter 0 (actual address)
    filter_in[0].filter_priority = 0;
    filter_in[0].filter_type = CAN_TYPE_EMERGENCY;
    filter_in[0].filter_cmd = 0;
    filter_in[0].filter_dest = moduleConfig.module_id;

	// init filter 1 (broadcast address)
	filter_in[1].filter_priority = 0;
    filter_in[1].filter_type = CAN_TYPE_EVENTS;
    filter_in[1].filter_cmd = 0;
    filter_in[1].filter_dest = CAN_ADDRESS_BROADCAST;
   
	// init can
   	can_init(filter_in,mask_in);
	

	//START TIMER1
	OpenTimer1(T1_ON & T1_GATE_OFF &
	T1_PS_1_256 & T1_SYNC_EXT_OFF &
	T1_SOURCE_INT, 39063);

	while(1)
	{
		//NORMAL MODE
		if (moduleConfig.module_state == BOOT_NORMAL)
		{

			//LED = ON
			PORTGbits.RG2 = 0;
			
			//LOOK FOR TIMER1 INTERRUPT
			//INCREMENT BOOT VARIABLE
			if (IFS0bits.T1IF)
			{
				//clear interrupt flag
				IFS0bits.T1IF = 0;
				
				//increment boot variable
				timer1Count++;
			}

			//LOOK FOR TIMEOUT
			//THEN JUMP INTO USER CODE AT 0x100 
			if (timer1Count == moduleConfig.boot_delay)
			{
				//JUMPING
				{__asm__ volatile ("goto 0x0100");}
			}
		}
		else 
		{
			//LED = OFF
			PORTGbits.RG2 = 1;
		}


		//GETTING CAN MESSAGES
		if (can_recv_laborius_packet(&g_rMessage) == 1)
		{

			//PROCESSING EVENTS
			if (g_rMessage.msg_type == CAN_TYPE_EVENTS)
			{
				//HANDLE ALIVE REQUESTS
				if (g_rMessage.msg_cmd == CAN_EVENTS_CMD_ALIVE)
				{
					can_send_im_alive(Can_Addr,&moduleConfig);
				}
			}
			//PROCESSING EMERGENCY
			else if (g_rMessage.msg_type == CAN_TYPE_EMERGENCY)
			{
				//HANDLE PROGRAMMING COMMANDS
				if (g_rMessage.msg_cmd == CAN_EMERGENCY_CMD_PROGRAM)
				{

					if (g_rMessage.msg_data_length == 0)
					{
						moduleConfig.module_state = BOOT_IDLE;
						writeBootConfig(&moduleConfig);
					}
					else if (g_rMessage.msg_data_length == 8)
					{
						//g_rMessage.msg_priority = 7;	
						//while(can_send_laborius_packet(&g_rMessage));
	
	 
						//FINITE STATE MACHINE FOR PROGRAMMING
						switch(moduleConfig.module_state)
						{
							case BOOT_IDLE:
	
								//LED = OFF
								PORTGbits.RG2 = 1;
	
								//make sure we reset those variables
								page_index = 0;
								base_address = 0;
	
								if (g_rMessage.msg_data[5] == CMD_NOP)
								{
									//NOT VERY USEFUL!
									Nop();
								}
								else if (g_rMessage.msg_data[5] == CMD_RESET)
								{
									//SOFTWARE RESET
									Reset();
								}
								else if (g_rMessage.msg_data[5] == CMD_START_WRITE)
								{
									//GET STARTING ADDRESS
									base_address = (unsigned long) g_rMessage.msg_data[0];
									base_address |= ((unsigned long)g_rMessage.msg_data[1]) << 8;
									base_address |= ((unsigned long)g_rMessage.msg_data[2]) << 16;
									//base_address |= ((unsigned long)g_rMessage.msg_data[3]) << 24;
	
									//SWITCHING TO WRITE MODE
									moduleConfig.module_state = BOOT_PROGRAM_FLASH;
								}
			
							break;
	
							case BOOT_PROGRAM_FLASH:
	
								//LED = ON
								PORTGbits.RG2 = 0;
	
								if (g_rMessage.msg_eeprom_ram) 
								{
									//READ 8 BYTES OF DATA
									memcpy(&flash_page[page_index % 64], &g_rMessage.msg_data[0], 8);
	
									//we have received 2 more instructions (2 words instructions = 4 words);								
									page_index += 4;
	
									//WRITE FLASH PAGE IF PAGE COMPLETE
									if (page_index % 64 == 0)
									{
										//clear index
										page_index = 0;
			
										//Write page at base_address
										WriteMem((unsigned int)(base_address >> 16), (unsigned int)(base_address & 0x0000FFFF), flash_page, 64);
	
										//clear page data
										memset(flash_page,0,64 * sizeof(unsigned int));
	
										//increment address
										base_address += 64; //a page is 64 words
									}
								}
								else if (g_rMessage.msg_data[5] == CMD_NOP)
								{
									Nop();
								}
								else if (g_rMessage.msg_data[5] == CMD_END_WRITE)
								{
									//WRITE NORMAL MODE TO EEPROM
									moduleConfig.module_state = BOOT_NORMAL;
									writeBootConfig(&moduleConfig);	
	
									//PUT MODULE BACK IN IDLE MODE TO RECEIVE
									//RESET COMMAND
									moduleConfig.module_state = BOOT_IDLE;
								}
								else
								{
									//UNRECOGNIZED COMMAND... GOING IDLE
									moduleConfig.module_state = BOOT_IDLE;
								}
							break;
	
							case BOOT_PROGRAM_EEPROM:
									//TODO : EEPROM IMPLEMENTATION
									//NOT SUPPORTED RIGHT NOW!
									moduleConfig.module_state = BOOT_IDLE;
							break;
	
							default:
									//INVALID MODE, GOING IN IDLE MODE
									moduleConfig.module_state = BOOT_IDLE;
							break;
						}//SWITCH BOOT_STATE
					}//CMD = PROGRAM
				}//MSG DATA LENGTH = 8
			}//TYPE = EMERGENCY
		} //IF RECV MESSAGE


	}

	return 0;
}

#endif

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
#include "NETV16_CANDriver.h"
#include <libpic30.h>
#include <timer.h>
#include "memory.h"
#include <string.h>


#ifdef _DSPIC30F5015_
//--------------------------Device Configuration------------------------
_FOSC(CSW_FSCM_OFF & XT_PLL8)
_FWDT(WDT_OFF)
_FBORPOR(PBOR_OFF & BORV45 & PWRT_64 & MCLR_EN &  PWMxL_ACT_HI & PWMxH_ACT_HI)
_FGS(GWRP_OFF & GCP_CODE_PROT_OFF)
_ICD(PGD & ICS_PGD)
//----------------------------------------------------------------------
#endif

#ifdef _DSPIC30F4011_
//--------------------------Device Configuration------------------------
_FOSC(CSW_FSCM_OFF & XT_PLL8)
_FWDT(WDT_OFF)
_FBORPOR(PBOR_OFF & BORV45 & PWRT_64 & MCLR_EN &  PWMxL_ACT_HI & PWMxH_ACT_HI)
_FGS(GWRP_OFF)
_ICD(PGD & ICS_PGD)
//----------------------------------------------------------------------
#endif


void init_led()
{

    #ifdef _DSPIC30F5015_
        #ifdef dsPICDrive
            TRISGbits.TRISG2 = 0; //G2 = OUTPUT
            LATGbits.LATG2 = 0; //TURN ON LED
        #else
            TRISBbits.TRISB13 = 0; //B13 = OUTPUT
            LATBbits.LATB13 = 0; //TURN ON LED
        #endif
    #endif


    #ifdef _DSPIC30F4011_
        TRISCbits.TRISC13 = 0; //C13 = OUTPUT
        LATCbits.LATC13 = 0; //TURN ON LED
    #endif
        
}


void toggle_led()
{

    #ifdef _DSPIC30F5015_
        #ifdef dsPICDrive
            LATGbits.LATG2 = ~LATGbits.LATG2;
        #else
            LATBbits.LATB13 = ~LATBbits.LATB13;
        #endif
    #endif


    #ifdef _DSPIC30F4011_
       LATCbits.LATC13 = ~LATCbits.LATC13; 
    #endif


}

void set_led(unsigned char state)
{
    #ifdef _DSPIC30F5015_
        #ifdef dsPICDrive
            LATGbits.LATG2 = state;
        #else
            LATBbits.LATB13 = state;
        #endif
    #endif


    #ifdef _DSPIC30F4011_
       LATCbits.LATC13 = state;
    #endif
    
}



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

		data[0] = config->module_state;
		data[1] = config->project_id;
		data[2] = config->module_id;
		data[3] = config->code_version;
		data[4] = config->table_version;
		data[5] = config->boot_delay;
                data[6] = config->devid_low;
                data[7] = config->devid_high;

		//WRITING BACK PAGE
		WriteMem(EEPROM_BASE_ADDRESS_HIGH,EEPROM_BASE_ADDRESS_LOW,data,16);
	}
}

void send_alive_answer(BootConfig *config)
{
    NETV_MESSAGE msg;
    int i = 0;

    msg.msg_priority = 0x00;
    msg.msg_type = NETV_TYPE_EVENTS;
    msg.msg_cmd = NETV_EVENTS_CMD_ALIVE;
    msg.msg_dest = config->module_id;
    msg.msg_eeprom_ram = NETV_REQUEST_EEPROM;
    msg.msg_read_write = NETV_REQUEST_READ;
    msg.msg_data_length = 8;
    msg.msg_remote = 0;

    //Fill module information
    //msg.msg_data[0] = config->module_state;
    msg.msg_data[0] = NETV_BOOT_IDLE;
    msg.msg_data[1] = config->project_id;
    msg.msg_data[2] = config->module_id;
    msg.msg_data[3] = config->code_version;
    msg.msg_data[4] = config->table_version;
    msg.msg_data[5] = config->boot_delay;
    msg.msg_data[6] = config->devid_low;
    msg.msg_data[7] = config->devid_high;

    while(netv_send_message(&msg));
}

//THIS IS CAUSING PROBLEMS UNDER MPLABX


/*
int __attribute__ ((space(eedata), aligned(16))) g_bootData[] = {
        BOOT_IDLE, //BOOT MODE
        0x0000, // PROJECT_ID
        0x0001, // MODULE_ID
        0x0001, // CODE_VERSION
        0x0002, // TABLE_VERSION
        0x0006, // BOOT_DELAY (SECONDS)
        0x0000, // DEVID LOW
        0x0000}; //DEVID HIGH
*/
 



//Bootloader commands
enum
{
    BOOTLOADER_SET_BASE_ADDR,
    BOOTLOADER_GET_BASE_ADDR,
    BOOTLOADER_SET_MODULE_ADDR,
    BOOTLOADER_GET_MODULE_ADDR,
    BOOTLOADER_SET_STATE,
    BOOTLOADER_GET_STATE,
    BOOTLOADER_SET_DELAY,
    BOOTLOADER_GET_DELAY,
    BOOTLOADER_READ_INC,
    BOOTLOADER_WRITE_INC,
    BOOTLOADER_READ_PAGE,
    BOOTLOADER_WRITE_PAGE,
    BOOTLOADER_WRITE_BOOTCONFIG,
    BOOTLOADER_RESET
};

#define BOOTLOADER_NACK 0b10000000;
#define PAGE_SIZE 64 //64 words (128 bytes)
#define WORD_SIZE 2  //1 word = 2 bytes
 
//This code should be working with multiple interface...
int main()
{
    unsigned int i = 0;
    unsigned int ready = 0;
    unsigned int timer1Count = 0;
    unsigned long baseAddr = 0;
    unsigned int pageOffset = 0;
    unsigned int pageData[PAGE_SIZE];
    BootConfig config;
    NETV_MESSAGE inputMessage;
    NETV_MASK mask_in[2];
    NETV_FILTER filter_in[6];

    //Read actual configuration
    readBootConfig(&config);

    //Initialize config for the first time
    if (config.module_id == 0xFF)
    {
        config.boot_delay = 6;
        config.table_version = 0x02;
        config.module_state = NETV_BOOT_IDLE;
        config.project_id = 0xFF;
        config.code_version = 0xFF;
        config.module_id = 1;
        
        //Write configuration
        writeBootConfig(&config);
    }


    init_led();

    //START TIMER1 (0.5 SEC & 20 MIPS)
    OpenTimer1(T1_ON & T1_GATE_OFF &
    T1_PS_1_256 & T1_SYNC_EXT_OFF &
    T1_SOURCE_INT, 39063);


    //init mask
    for(i=0;i<2;i++){
        mask_in[i].mask_priority = 0;
        mask_in[i].mask_type = 0;
        mask_in[i].mask_cmd = 0;
        mask_in[i].mask_dest = 0xFF;
    }

    //init filter
    for(i=0;i<6;i++){
        filter_in[i].filter_priority = 0;
        filter_in[i].filter_type = 0;
        filter_in[i].filter_cmd = 0;
        filter_in[i].filter_dest = config.module_id;
    }

    //init can
    netv_init_can_driver(filter_in,mask_in);

    while(1)
    {
        //RECEIVE CAN MESSAGES
        if(netv_recv_message(&inputMessage))
        {
            if (inputMessage.msg_type == NETV_TYPE_EVENTS)
            {
                //HANDLE ALIVE REQUESTS
                if (inputMessage.msg_cmd == NETV_EVENTS_CMD_ALIVE)
                {
                   send_alive_answer(&config);
                }
            }
            else if (inputMessage.msg_type == NETV_TYPE_EMERGENCY)
            {
                
            }
            else if (inputMessage.msg_type == NETV_TYPE_BOOTLOADER)
            {

                //Toggle led
                toggle_led();

                //Reset timer
                timer1Count = 0;

                NETV_MESSAGE answerMessage = inputMessage;


                //HANDLE EMERGENCY COMMANDS
                switch (inputMessage.msg_cmd)
                {
                    case BOOTLOADER_SET_BASE_ADDR:

                        //As soon as we receive a bootloader message
                        //We are in bootloader mode
                        config.module_state = NETV_BOOT_IDLE;

                        //GET ADDR
                        baseAddr = (unsigned long) inputMessage.msg_data[0];
                        baseAddr |= ((unsigned long) inputMessage.msg_data[1]) << 8;
                        baseAddr |= ((unsigned long) inputMessage.msg_data[2]) << 16;
                        baseAddr |= ((unsigned long) inputMessage.msg_data[3]) << 24;

                        //Starting at the beginning of the buffer
                        pageOffset = 0;

                        ready = 1;

                        //Load FLASH
                        if (baseAddr >= BOOTLOADER_ADDRESS)
                        {
                            //NACK
                            answerMessage.msg_cmd |= BOOTLOADER_NACK;
                        }
                        break;

                    case BOOTLOADER_GET_BASE_ADDR:
                        answerMessage.msg_remote = 0;
                        answerMessage.msg_data_length = 4;
                        answerMessage.msg_data[0] = (unsigned char)((unsigned long) BOOTLOADER_ADDRESS >> 24);
                        answerMessage.msg_data[1] = (unsigned char)((unsigned long) BOOTLOADER_ADDRESS >> 16);
                        answerMessage.msg_data[2] = (unsigned char)((unsigned long) BOOTLOADER_ADDRESS >> 8);
                        answerMessage.msg_data[3] = (unsigned char)((unsigned long) BOOTLOADER_ADDRESS);
                        break;

                    case BOOTLOADER_SET_MODULE_ADDR:
                        if (inputMessage.msg_data_length == 1)
                        {                          
                            //Send answer & Apply & Reset NOW!
                            while(netv_send_message(&answerMessage));
							
							config.module_id = inputMessage.msg_data[0];
                            //Write config
                            writeBootConfig(&config);
							
                            __delay_ms(50);
                            asm("RESET");

                        }
                        else
                        {
                             //NACK
                            answerMessage.msg_cmd |= BOOTLOADER_NACK;
                        }
                        break;
                        
                    case BOOTLOADER_GET_MODULE_ADDR:
                        answerMessage.msg_remote = 0;
                        answerMessage.msg_data_length = 1;
                        answerMessage.msg_data[0] = config.module_id;
                        break;

                    case BOOTLOADER_SET_STATE:

                        if (inputMessage.msg_data_length == 1)
                        {
                            config.module_state = inputMessage.msg_data[0];
                        }
                        else
                        {
                            //NACK
                            answerMessage.msg_cmd |= BOOTLOADER_NACK;
                        }
                        break;

                    case BOOTLOADER_GET_STATE:
                        //RETURN STATE
                        answerMessage.msg_remote = 0;
                        answerMessage.msg_data_length = 1;
                        answerMessage.msg_data[0] = config.module_state;
                        break;

                    case BOOTLOADER_SET_DELAY:
                        if(inputMessage.msg_data_length == 1)
                        {
                            config.boot_delay = inputMessage.msg_data[0];
                        }
                        else
                        {
                             //NACK
                            answerMessage.msg_cmd |= BOOTLOADER_NACK;
                        }
                        break;

                    case BOOTLOADER_GET_DELAY:
                        //RETURN DELAY
                        answerMessage.msg_remote = 0;
                        answerMessage.msg_data_length = 1;
                        answerMessage.msg_data[0] = config.boot_delay;
                        break;

                    case BOOTLOADER_READ_INC:
                            //NACK (NOT YET IMPLEMENTED)
                            answerMessage.msg_cmd |= BOOTLOADER_NACK;
                        break;

                    case BOOTLOADER_WRITE_INC:
                        //make sure we don't overflow our buffer
                        if (pageOffset + (inputMessage.msg_data_length) / WORD_SIZE <= PAGE_SIZE)
                        {
                            memcpy(&pageData[pageOffset],inputMessage.msg_data, inputMessage.msg_data_length);
                            pageOffset += (inputMessage.msg_data_length) / WORD_SIZE;
                        }
                        else
                        {
                            answerMessage.msg_cmd |= BOOTLOADER_NACK;
                        }
                            
                        //time to write (except for reset vector)
                        if (pageOffset % PAGE_SIZE == 0)
                        {

                            //Watch out for reset vector, must not overrite it
                            if (baseAddr == 0 && ready)
                            {

                                pageData[0] =  BOOTLOADER_ADDRESS;
                                
                                //Make sure we dont't boot in normal mode
                                //if data transfer aborts
                                config.module_state =  NETV_BOOT_IDLE;
                                writeBootConfig(&config);
                            }


                            //Make sure we don't overwrite the bootloader
                            if (baseAddr < BOOTLOADER_ADDRESS && ready)
                            {
                                //Writing
                                //WriteMem(unsigned int addr_high, unsigned int addr_low, unsigned int* dataPtr, unsigned int size);
                                WriteMem((unsigned int)(baseAddr >> 16),(unsigned int) (baseAddr & 0x0000FFFF), pageData,PAGE_SIZE);

                                //Next page
                                baseAddr += PAGE_SIZE;
                                pageOffset = 0;
                            }
                            else
                            {
                                //NACK
                                answerMessage.msg_cmd |= BOOTLOADER_NACK;
                            }
                        }
                        break;

                    case BOOTLOADER_READ_PAGE:
                        //NACK (NOT YET IMPLEMENTED)
                        answerMessage.msg_cmd |= BOOTLOADER_NACK;
                        break;

                    case BOOTLOADER_WRITE_PAGE:
                        //NACK (NOT YET IMPLEMENTED)
                        answerMessage.msg_cmd |= BOOTLOADER_NACK;
                        break;

                    case BOOTLOADER_WRITE_BOOTCONFIG:
                        writeBootConfig(&config);
                        break;

                    case BOOTLOADER_RESET:

                        while(netv_send_message(&answerMessage));

                        __delay_ms(50);
                        
                        //RESET!
                        asm("RESET");
                        //ACK
                        break;
                }


               //Send ACK / NACK / ANSWER
               while(netv_send_message(&answerMessage));
                
                
            } //Bootloader type
        } //RECV CAN MESSAGE

        //HANDLE NORMAL MODE
        if (config.module_state == NETV_BOOT_NORMAL)
        {
                //LOOK FOR TIMEOUT
                //THEN JUMP INTO USER CODE AT 0x100
                if (timer1Count >= config.boot_delay)
                {
                        //LED OFF
                        set_led(0);

                        //JUMPING to standard location
                        {__asm__ volatile ("goto 0x0100");}
                }
        } //BOOT NORMAL

        //Timer1 interrupt flag
        if (IFS0bits.T1IF)
        {
            //clear interrupt flag
            IFS0bits.T1IF = 0;

            //Toggle led
            toggle_led();

            //increment timer counter
            timer1Count++;
        }
    }

    return 0;
}



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

// Library call
#include "NETV8_Shared.h"
#include "NETV8_Device.h"
#include "NETV8_Common.h"

#include <delays.h>

// Prototypes
unsigned char netv_write_data_flow_table_v2(unsigned int offset, unsigned char mem_type, unsigned char *buffer, unsigned int size);
unsigned char netv_read_data_flow_table_v2(unsigned int offset, unsigned char mem_type, unsigned char *buffer, unsigned int size);


//GLOBAL BOOT CONFIGURATION USED FOR THIS DEVICE
BootConfig g_BootConfig;

//fit the size of the data flow table to the global variable structure
#define DATA_FLOW_TABLE_SIZE (sizeof(GlobalNETVVariables))

//Const pointer to changeable data
volatile unsigned char * const DATA_FLOW_TABLE = (unsigned char*) &g_globalNETVVariables;

/*******************************************************************
READ EEPROM
 *******************************************************************/
unsigned char netv_read_eeprom(unsigned char addr) {
    volatile unsigned char eepTemp;
    EEADR = addr;
    EECON1bits.EEPGD = 0;
    EECON1bits.CFGS = 0;
    EECON1bits.RD = 1;
    eepTemp = EEDATA;
    return eepTemp;
}

/*******************************************************************
WRITE EEPROM
 *******************************************************************/
void netv_write_eeprom(unsigned char addr, unsigned char data) {
    INTCONbits.GIEH = 0; //disable interrupts	
    EECON1bits.EEPGD = 0; //EEPROM ACCESS
    EECON1bits.CFGS = 0; //FLASH OR EEPROM
    EECON1bits.WREN = 1; //WRITE ENABLE
    EEADR = addr; //SET ADDRESS
    EEDATA = data; //SET DATA
    EECON2 = 0x55;
    EECON2 = 0xaa;
    EECON1bits.WR = 1; //WRITE
    while (EECON1bits.WR == 1);
    //enable interrupts
    INTCONbits.GIEH = 1; //Enable interrupts
}


//////////////////////////////////////////////////////////////////////
//   netv_transceiver()
//////////////////////////////////////////////////////////////////////
//
//   Description: MCP2510 unsigned charerrupt sub-routine
//                Handles all MCP2510 unsigned charerruptions until none are left
//
//   Input: NONE
//   Output: NONE
//   Input/Output: NONE
//   Returned value: NONE
//
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void netv_transceiver(unsigned char netv_addr) {
    NETV_MESSAGE g_rMessage;
    unsigned char buffer_size = 0;
    unsigned int offset = 0;



    while (netv_recv_message(&g_rMessage))
    {
        //Analyse for boot mode and I'm alive signal
        switch (g_rMessage.msg_type) {
            case NETV_TYPE_EMERGENCY:

                switch (g_rMessage.msg_cmd) {
                    case NETV_EMERGENCY_CMD_PROGRAM:
                        //indicate to the bootloader that we are gonna program

                        //TODO Update Boot Config
                        //netv_write_eeprom(0xFF,NETV_BOOT_MODE_ID);

                        //wait 10ms
                        Delay10KTCYx(10);

                        //reset!
                        Reset();
                        break;
                }//end switch msg_cmd

                break;

            case NETV_TYPE_EVENTS:
                switch (g_rMessage.msg_cmd) {
                    case NETV_EVENTS_CMD_ALIVE:
                        //Send i'm alive
                        netv_send_im_alive(netv_addr);
                        break;
                }//end switch msg_cmd
                break;

            case NETV_TYPE_REQUEST_DATA:
                if (g_rMessage.msg_dest == netv_addr)
                {
                    //get the memory offset
                    offset = g_rMessage.msg_cmd;

                    switch (g_rMessage.msg_read_write) {
                        case NETV_REQUEST_READ:

                            //WE MUST RECEIVE A REMOTE REQUEST...
                            if (g_rMessage.msg_remote == 1) {
                                netv_read_data_flow_table_v2(offset,
                                        g_rMessage.msg_eeprom_ram,
                                        &g_rMessage.msg_data[0],
                                        MIN(g_rMessage.msg_data_length, 8));

                                g_rMessage.msg_remote = 0;

                                while (netv_send_message(&g_rMessage)) {
                                    ;
                                }

                            }

                            break;

                        case NETV_REQUEST_WRITE:
                            // We don't want to overwrite EEPROM table information
                            // The message must not be a remote request
                            if (g_rMessage.msg_remote == 0) {
                                netv_write_data_flow_table_v2(offset,
                                        g_rMessage.msg_eeprom_ram,
                                        &g_rMessage.msg_data[0],
                                        MIN(g_rMessage.msg_data_length, 8));
                            }
                            break;

                    }//end sub-switch read_write
                }
                break;
        }
        // processe the received message
        netv_proc_message(&g_rMessage);
    }
}



//////////////////////////////////////////////////////////////////////
//   netv_send_im_alive
//////////////////////////////////////////////////////////////////////
//
//   Description: Send a I'M Alive can message
//
//   Input: netv_addr (Address read in the EEPROM at 0XFE)
//   Output: NONE
//   Input/Output: NONE
//   Returned value: NONE
//
//////////////////////////////////////////////////////////////////////

void netv_send_im_alive(unsigned char netv_addr) {
    NETV_MESSAGE msg;
    int i = 0;

    msg.msg_priority = 0x00;
    msg.msg_type = NETV_TYPE_EVENTS;
    msg.msg_cmd = NETV_EVENTS_CMD_ALIVE;
    msg.msg_dest = netv_addr;
    msg.msg_eeprom_ram = NETV_REQUEST_EEPROM;
    msg.msg_read_write = NETV_REQUEST_READ;
    msg.msg_data_length = 8;

    //Protocol version 2
    for (i = 0; i < 8; i++) {
        msg.msg_data[i] = netv_read_eeprom(i);
    }

    //make sure we are returning the right netv_addr
    msg.msg_data[2] = netv_addr;

    msg.msg_remote = 0;
    netv_send_message(&msg);
}

//////////////////////////////////////////////////////////////////////
//   read_data_flow_table_v2
//////////////////////////////////////////////////////////////////////
//
//   Description: Read "size" byte(s) from DATA_FLOW_TABLE
//
//   Input: 
//			 offset  - table index
//			 mem_type - eeprom or ram
//			 buffer - buffer address whre to store the read data
//			 size - the size of the read
//   Output: read succesfull/not succesfull,If the reading is successfull "1" is returned else "0"
//
//////////////////////////////////////////////////////////////////////

unsigned char netv_read_data_flow_table_v2(unsigned int offset, unsigned char mem_type, unsigned char *buffer, unsigned int size) {
    unsigned int i = 0;
    unsigned char success = 1;

    switch (mem_type) {
        case NETV_REQUEST_RAM:
            if (offset + size <= DATA_FLOW_TABLE_SIZE) {
                INTCONbits.GIEH = 0; //disable interrupts
                for (i = offset; i < (offset + size); i++) {
                    buffer[i - offset] = DATA_FLOW_TABLE[i];
                }
                INTCONbits.GIEH = 1; //enable interrupts

                success = 1;
            } else {
                //memory out of bound
                success = 0;
            }
            break;

        case NETV_REQUEST_EEPROM:
            if (offset + size <= DATA_FLOW_TABLE_SIZE) {
                INTCONbits.GIEH = 0; //disable interrupts
                for (i = offset; i < (offset + size); i++) {
                    buffer[i - offset] = netv_read_eeprom(i);
                }
                INTCONbits.GIEH = 1; //enable interrupts

                success = 1;
            } else {
                //memory out of bound
                success = 0;
            }
            break;

        default:
            success = 0;
            break;
    }

    return success;
}


//////////////////////////////////////////////////////////////////////
//   write_data_flow_table
//////////////////////////////////////////////////////////////////////
//
//   Description: Write "size" byte(s) to DATA_FLOW_TABLE
//
//   Input: 
//			 offset  - table index
//			 mem_type - eeprom or ram
//			 buffer - buffer address where to get the data
//			 size - the size of the write
//   Output: read succesfull/not succesfull,If the reading is successfull "1" is returned else "0"
//
//////////////////////////////////////////////////////////////////////

unsigned char netv_write_data_flow_table_v2(unsigned int offset, unsigned char mem_type, unsigned char *buffer, unsigned int size) {

    unsigned int i = 0;
    unsigned char success = 1;

    switch (mem_type) {
        case NETV_REQUEST_RAM:
            if (offset + size <= DATA_FLOW_TABLE_SIZE) {
                INTCONbits.GIEH = 0; //disable interrupts
                for (i = offset; i < (offset + size); i++) {
                    DATA_FLOW_TABLE[i] = buffer[i - offset];
                }
                INTCONbits.GIEH = 1; //enable interrupts

                success = 1;
            } else {
                //memory out of bound
                success = 0;
            }
            break;


        case NETV_REQUEST_EEPROM:
            //must protect firt 8 bytes.
            if ((offset + size <= DATA_FLOW_TABLE_SIZE) && (offset >= 8)) {
                INTCONbits.GIEH = 0; //disable interrupts
                for (i = offset; i < (offset + size); i++) {
                    netv_write_eeprom(i, buffer[i - offset]);
                }
                INTCONbits.GIEH = 1; //enable interrupts

                success = 1;
            } else {
                //memory out of bound
                success = 0;
            }
            break;

        default:
            success = 0;
            break;
    }

    return success;
}

unsigned char table_read(void) {

    // _asm
    //   TBLRDPOSTINC
    //_endasm
    return TABLAT;
}

void netv_read_boot_config(BootConfig *config) {
    unsigned int devid = 0;

    if (config) {
        config->module_state = netv_read_eeprom(0);
        config->project_id = netv_read_eeprom(1);
        config->module_id = netv_read_eeprom(2);
        config->code_version = netv_read_eeprom(3);
        config->table_version = netv_read_eeprom(4);
        config->boot_delay = netv_read_eeprom(5);

        //read devid
        TBLPTR = DEVID_BASE_ADDRESS;
        config->devid_low = table_read();
        config->devid_high = table_read();
    }
}

void netv_write_boot_config(BootConfig *config) {
    unsigned char i = 0;

    if (config) {

        unsigned int data[8];

        data[0] = config->module_state;
        data[1] = config->project_id;
        data[2] = config->module_id;
        data[3] = config->code_version;
        data[4] = config->table_version;
        data[5] = config->boot_delay;

        //DEVID DOES NOT NEED TO BE WRITTEN, BUT WE DO IT ANYWAY!
        TBLPTR = DEVID_BASE_ADDRESS;
        data[6] = table_read();
        data[7] = table_read();

        for (i = 0; i < 8; i++) {
            netv_write_eeprom(i, data[i]);
        }
    }
}

BootConfig* netv_get_boot_config(void) {
    return &g_BootConfig;
}



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

#ifndef _CAN18_DRIVER_H_
#define _CAN18_DRIVER_H_

#include "CAN18_Shared.h"
#include "CAN18_Device.h"


/*
MESSAGE PRIORITY DEFINITIONS
*/
#define CAN_PRIORITY_HIGHEST     0
#define CAN_PRIORITY_HIGH        1
#define CAN_PRIORITY_MEDIUM_HIGH 2
#define CAN_PRIORITY_MEDIUM      3
#define CAN_PRIORITY_MEDIUM_LOW  4
#define CAN_PRIORITY_LOW         5
#define CAN_PRIORITY_LOWEST      6
#define CAN_PRIORITY_EVENTS      7

/*
MESSAGE TYPE DEFINITIONS
*/
#define CAN_TYPE_EMERGENCY                0x01
#define CAN_TYPE_ACTUATOR_HIGH_PRIORITY   0x02
#define CAN_TYPE_SENSOR_HIGH_PRIORITY     0x04
#define CAN_TYPE_ACTUATOR_LOW_PRIORITY    0x08
#define CAN_TYPE_SENSOR_LOW_PRIORITY      0x10
#define CAN_TYPE_REQUEST_DATA             0x20
#define CAN_TYPE_USER2                    0x40
#define CAN_TYPE_EVENTS                   0x80

/*
EMERGENCY MESSAGE COMMANDS (TYPE_EMERGENCY)
*/
#define CAN_EMERGENCY_CMD_RESET        0x00
#define CAN_EMERGENCY_CMD_STOP         0x01
#define CAN_EMERGENCY_CMD_DISCONNECT   0x02
#define CAN_EMERGENCY_CMD_RECONNECT    0x03
#define CAN_EMERGENCY_CMD_PROGRAM      0x04

//BOOT-MEMORY FLAGS
#define CAN_REQUEST_READ                0x01
#define CAN_REQUEST_WRITE               0x00
#define CAN_REQUEST_EEPROM              0x01
#define CAN_REQUEST_RAM                 0x00

/*
EVENTS MESSAGE COMMANDS (TYPE EVENTS)
*/
#define CAN_EVENTS_CMD_ALIVE           0x00
#define CAN_EVENTS_CMD_REGISTER        0x01  

/*
CAN ADDRESS DEFINITIONS
*/
#define CAN_ADDRESS_BROADCAST 0xFF

/*
STRUCTURES
*/

typedef struct _message{
   unsigned char msg_priority;
   unsigned char msg_type;
   unsigned char msg_cmd;
   unsigned char msg_dest;
   unsigned char msg_read_write;
   unsigned char msg_eeprom_ram;
   unsigned char msg_data_length;
   unsigned char msg_data[8];
   unsigned char msg_remote;
} CAN_MESSAGE;

typedef struct _filter {
   unsigned char filter_priority;
   unsigned char filter_type;
   unsigned char filter_cmd;
   unsigned char filter_dest;
} CAN_FILTER;

typedef struct _mask {
   unsigned char mask_priority;
   unsigned char mask_type;
   unsigned char mask_cmd;
   unsigned char mask_dest;
} CAN_MASK;

typedef struct _BootConfig
{
	//WILL BE WRITTEN TO EEPROM
	unsigned char module_state;
	unsigned char project_id;
	unsigned char module_id;
	unsigned char code_version;
	unsigned char table_version;
	unsigned char boot_delay;

	//READ ONLY DEVICE ID
	unsigned char devid_low;
	unsigned char devid_high;		
} BootConfig;


//MODULE STATES
enum {BOOT_IDLE=0x0A, BOOT_PROGRAM_FLASH=0x01, BOOT_PROGRAM_EEPROM=0x02, BOOT_NORMAL=0x05};


//Function prototypes
void can_init(CAN_FILTER *filter,CAN_MASK *mask);
void can_apply_accept_mask(CAN_MASK *mask, unsigned char mask_id);
void can_apply_filter(CAN_FILTER *filter, unsigned char filter_id);
unsigned char can_send_message(CAN_MESSAGE *message);
unsigned char can_recv_message(CAN_MESSAGE *message);
void can_send_im_alive(unsigned char can_addr);
void can_transceiver(unsigned char can_addr);
void can_write_eeprom(unsigned char addr, unsigned char data);
unsigned char can_read_eeprom(unsigned char addr);

//This function needs to be defined in your code to handle custom messages
extern void can_proc_message(CAN_MESSAGE *message);

//Boot configuration
BootConfig* can_get_boot_config(void);
void can_read_boot_config(BootConfig *config);
void can_write_boot_config(BootConfig *config);



#endif

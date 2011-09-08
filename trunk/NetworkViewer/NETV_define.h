/**
     Copyright (C) 2009-2010 IntRoLab
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

#ifndef _NETV_DEFINE_H_
#define _NETV_DEFINE_H_



/**
    MESSAGE PRIORITY DEFINITIONS
*/
#define NETV_PRIORITY_HIGHEST     0
#define NETV_PRIORITY_HIGH        1
#define NETV_PRIORITY_MEDIUM_HIGH 2
#define NETV_PRIORITY_MEDIUM      3
#define NETV_PRIORITY_MEDIUM_LOW  4
#define NETV_PRIORITY_LOW         5
#define NETV_PRIORITY_LOWEST      6
#define NETV_PRIORITY_EVENTS      7

/**
    MESSAGE TYPE DEFINITIONS
*/
#define NETV_TYPE_EMERGENCY                0x01
#define NETV_TYPE_ACTUATOR_HIGH_PRIORITY   0x02
#define NETV_TYPE_SENSOR_HIGH_PRIORITY     0x04
#define NETV_TYPE_ACTUATOR_LOW_PRIORITY    0x08
#define NETV_TYPE_SENSOR_LOW_PRIORITY      0x10
#define NETV_TYPE_REQUEST_DATA             0x20
#define NETV_TYPE_BOOTLOADER               0x40
#define NETV_TYPE_EVENTS                   0x80
#define NOTHING                           0xFF

/**
    EMERGENCY MESSAGE COMMANDS (TYPE_EMERGENCY)
*/
#define NETV_EMERGENCY_CMD_RESET        0x00
#define NETV_EMERGENCY_CMD_STOP         0x01
#define NETV_EMERGENCY_CMD_DISCONNECT   0x02
#define NETV_EMERGENCY_CMD_RECONNECT    0x03
#define NETV_EMERGENCY_CMD_PROGRAM      0x04

#define NETV_REQUEST_READ                0x01
#define NETV_REQUEST_WRITE               0x00

#define NETV_REQUEST_EEPROM              0x01
#define NETV_REQUEST_RAM                 0x00

/**
    EVENTS MESSAGE COMMANDS (TYPE EVENTS)
*/
#define NETV_EVENTS_CMD_ALIVE           0x00
#define NETV_EVENTS_CMD_REGISTER        0x01  // unused
#define NETV_EVENTS_CMD_SILENCE         0x02

/**
    NETV ADDRESS DEFINITIONS
*/
#define NETV_ADDRESS_BROADCAST 0xFF


/**
    NETV MODULE STATES
*/
#define NETV_NORMAL_MODE_ID          0x05
#define NETV_BOOT_MODE_ID            0x0A
#define NETV_IDLE_MODE_ID            0x0B


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

#endif

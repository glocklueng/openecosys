/**
     Copyright (C) 2009-2010 IntRoLab
     http://introlab.gel.usherbrooke.ca
     Dominic Letourneau, ing. M.Sc.A.
     Dominic.Letourneau@USherbrooke.ca
 */

#ifndef _CAN_DEFINE_H_
#define _CAN_DEFINE_H_



/**
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

/**
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
#define NOTHING                           0xFF

/**
    EMERGENCY MESSAGE COMMANDS (TYPE_EMERGENCY)
*/
#define CAN_EMERGENCY_CMD_RESET        0x00
#define CAN_EMERGENCY_CMD_STOP         0x01
#define CAN_EMERGENCY_CMD_DISCONNECT   0x02
#define CAN_EMERGENCY_CMD_RECONNECT    0x03
#define CAN_EMERGENCY_CMD_PROGRAM      0x04

#define CAN_REQUEST_READ                0x01
#define CAN_REQUEST_WRITE               0x00

#define CAN_REQUEST_EEPROM              0x01
#define CAN_REQUEST_RAM                 0x00

/**
    EVENTS MESSAGE COMMANDS (TYPE EVENTS)
*/
#define CAN_EVENTS_CMD_ALIVE           0x00
#define CAN_EVENTS_CMD_REGISTER        0x01  // unused
#define CAN_EVENTS_CMD_SILENCE         0x02

/**
    CAN ADDRESS DEFINITIONS
*/
#define CAN_ADDRESS_BROADCAST 0xFF


/**
    CAN MODULE STATES
*/
#define CAN_NORMAL_MODE_ID          0x05
#define CAN_BOOT_MODE_ID            0x0A
#define CAN_IDLE_MODE_ID            0x0B

#endif

#ifndef _CAN18_SERIAL_H_
#define _CAN18_SERIAL_H_

typedef struct
{
    unsigned long sid;
    /*
    // SID of the Received CAN Message.
    unsigned long SID:11;

    //CAN interface number
    unsigned long IFACE:5;

    // Time stamp of the received message. This is
    // valid only if the Timestamping is enabled.
    unsigned long CMSGTS:16;
    */
}CAN_RX_MSG_SID;

// *****************************************************************************
/* CAN TX Message SID Word

  Summary:
    Defines the structure of the SID word section of the TX message.

  Description:
    This data structure represents the SID section of the CAN TX message. The data
    data structure is an element of the CANTxMessageBuffer data structure.
*/

typedef	union
{
    /*

    struct {
        struct
        {
            unsigned SID2:8;
        };

        struct
        {
            unsigned SID1:3;
            unsigned IFACE:5;
        };

        struct
        {
            unsigned :8;
        };

        struct
        {
            unsigned :8;
        };
    };

     */

    unsigned char bytes[4];
    unsigned long sid;
    /*
    // CAN TX Message Standard ID. This value should
    // be between 0x0 - 0x7FF.
    unsigned SID:11;

    //CAN interface number
    unsigned IFACE:5;

    unsigned :16;
    */
}CAN_TX_MSG_SID;

typedef union
{
    //Mapping to 8 bits registers...
    /*
    struct {
        union {
            struct {
                unsigned DLC:4;
                unsigned RB0:1;
                unsigned :3;
            };
            unsigned char byte3;
        };

        union {
            struct {
                unsigned RB1:1;
                unsigned RTR:1;
                unsigned EID3:6;
            };
            unsigned char byte2;
        };

        union {
            struct {
                unsigned EID2:8;
            };

            unsigned char byte1;
        };

        union {
            struct {
                unsigned EID1:4;
                unsigned IDE:1;
                unsigned SRR:1;
                unsigned :2;
            };
            unsigned char byte0;
        };
    };
    */
    
    //PIC32 registers...
    /*

    // Data Length Control. Specifies the size of the
    // data payload section of the CAN packet. Valid
    // values are 0x0 - 0x8.
    unsigned long DLC:4;

    // Reserved bit. Should be always 0.
    unsigned long RB0:1;
    unsigned long :3;

    // Reserved bit. Should be always 0.
    unsigned long RB1:1;

    // Remote Transmit Request bit. Should be set for
    // RTR messages, clear otherwise.
    unsigned long RTR:1;

    // CAN TX and RX Extended ID field. Valid values
    // are in range 0x0 - 0x3FFFF.
    unsigned long EID:18;

    // Identifier bit. If 0 means that message is SID.
    // If 1 means that message is EID type.
    unsigned long IDE:1;

    // Susbtitute Remote request bit. This bit should
    // always be clear for an EID message. It is ignored
    // for an SID message.
    unsigned long SRR:1;
    unsigned long :2;

     */
    unsigned char bytes[4];
    unsigned long eid;
}CAN_MSG_EID;

typedef union {

        struct
        {
            // This is SID portion of the CAN RX message. (4 bytes)
            CAN_RX_MSG_SID msgSID;

            // This is EID portion of the CAN RX message (4 bytes)
            CAN_MSG_EID msgEID;

            // This is the data payload section of the
            // received message.
            unsigned char data[8];
        };

    // This is CAN RX message organized as a set of 32 bit
    // words.
    unsigned char messageBytes[16];

}CANRxMessageBuffer;

typedef union {

    struct
    {
        // This is SID portion of the CAN TX message.
        CAN_TX_MSG_SID msgSID;

        // This is EID portion of the CAN TX message.
        CAN_MSG_EID msgEID;

        // This is the data portion of the CAN TX message.
        unsigned char data[8];
    };

    // This is CAN TX message organized as a set of 32 bit
    // words.
    unsigned char messageBytes[16];

}CANTxMessageBuffer;


//Data structure that we internally use for messages
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

//Function prototypes
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

#define RX_BUFFER_SIZE 5
#define TX_BUFFER_SIZE 5

//Serial functions


#endif



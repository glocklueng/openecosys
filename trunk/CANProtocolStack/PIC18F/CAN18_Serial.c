#include "CAN18_Serial.h"
#include "CAN18_Device.h"
#include "CAN18_Shared.h"
#include "CAN18_Utils.h"
#include <usart.h>
#include <delays.h>

// GLOBAL VARIABLES TO BE USED IN ALL SOURCE FILES
CAN_MESSAGE g_rMessage = {0}; // all downloaded messages from PIC
// are written here


//CANRxMessageBuffer g_RxMessageBuffer[RX_BUFFER_SIZE];
//CANTxMessageBuffer g_TxMessageBuffer[TX_BUFFER_SIZE];



// Prototypes
unsigned char can_write_data_flow_table_v2(unsigned int offset, unsigned char mem_type, unsigned char *buffer, unsigned int size);
unsigned char can_read_data_flow_table_v2(unsigned int offset, unsigned char mem_type, unsigned char *buffer, unsigned int size);


//GLOBAL BOOT CONFIGURATION USED FOR THIS DEVICE
BootConfig g_BootConfig;

//fit the size of the data flow table to the global variable structure
#define DATA_FLOW_TABLE_SIZE (sizeof(GlobalCANVariables))

//Const pointer to changeable data
volatile unsigned char * const DATA_FLOW_TABLE = (unsigned char*) &g_globalCANVariables;

/*******************************************************************
READ EEPROM
 *******************************************************************/
unsigned char can_read_eeprom(unsigned char addr) {
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
void can_write_eeprom(unsigned char addr, unsigned char data) {
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





/* LABORIUS SECTION ########################################################################################### */

//////////////////////////////////////////////////////////////////////
//   can_send_message
//////////////////////////////////////////////////////////////////////
//
//   Description: Fills a TX buffer with a modified message and sends
//                a Request To Send.
//
//   Input: object_id specifying the TX buffer to use
//   Output: NONE
//   Input/Output: message
//   Returned value: NONE
//
//////////////////////////////////////////////////////////////////////

unsigned char can_send_message(CAN_MESSAGE *message)
{
    unsigned char i=0;

    //Need to transform a TxMessageBuffer into a CAN_MESSAGE
    CANTxMessageBuffer buf;

    //TODO Convert data structures...
    buf.msgEID.bytes[3] = 0;
    buf.msgEID.bytes[2] = 0;
    buf.msgEID.bytes[1] = 0;
    buf.msgEID.bytes[0] = 0;

    buf.msgSID.bytes[3] = 0;
    buf.msgSID.bytes[2] = 0;
    buf.msgSID.bytes[1] = 0;
    buf.msgSID.bytes[0] = 0;

    //copy data
    for (i = 0 ; i < 8; i++)
    {
        buf.data[i] = message->msg_data[i];
    }


    //Transmit on serial
    //Right now it will be synchronous, need to be event based with interrupts
    for (i = 0; i < sizeof(CANTxMessageBuffer); i++)
    {
        putcUSART2(buf.messageBytes[i]);
    }

    return 1;
}

//////////////////////////////////////////////////////////////////////
//   can_recv_message
//////////////////////////////////////////////////////////////////////
//
//   Description: Extract RX buffer message and put it in a message
//                structure.
//
//   Input: object_id specifying the TX buffer to use
//   Output: NONE
//   Input/Output: message
//   Returned value: NONE
//
//////////////////////////////////////////////////////////////////////

unsigned char can_recv_message(CAN_MESSAGE *message)
{
    unsigned char i = 0;
    //Need to transform a RxMessageBuffer into a CAN_MESSAGE
    CANRxMessageBuffer buf;

    //Get all data, this needs to be interrupt based...
    getsUSART2(&buf.messageBytes[0],sizeof(CANRxMessageBuffer));

    //TODO Convert data structures
    message->msg_priority = 0;
    message->msg_type = 0;
    message->msg_eeprom_ram = 0;
    message->msg_read_write = 0;
    message->msg_cmd = 0;
    message->msg_dest = 0;


    message->msg_remote = 0;
    message->msg_data_length = 0;

    //copy data
    for(i = 0; i < 8; i++)
    {
        message->msg_data[i] = buf.data[i];
    }

    return 1;
}

//////////////////////////////////////////////////////////////////////
//   can_transceiver()
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

void can_transceiver(unsigned char can_addr) {
    CAN_MESSAGE msg_value;
    unsigned char buffer_size = 0;
    unsigned int offset = 0;


    while (can_recv_message(&g_rMessage)) {
        //Analyse for boot mode and I'm alive signal
        switch (g_rMessage.msg_type) {
            case CAN_TYPE_EMERGENCY:

                switch (g_rMessage.msg_cmd) {
                    case CAN_EMERGENCY_CMD_PROGRAM:
                        //indicate to the bootloader that we are gonna program

                        //TODO Update Boot Config
                        //can_write_eeprom(0xFF,CAN_BOOT_MODE_ID);

                        //wait 10ms
                        Delay10KTCYx(10);

                        //reset!
                        Reset();
                        break;
                }//end switch msg_cmd

                break;

            case CAN_TYPE_EVENTS:
                switch (g_rMessage.msg_cmd) {
                    case CAN_EVENTS_CMD_ALIVE:
                        //Send i'm alive
                        can_send_im_alive(can_addr);
                        break;
                }//end switch msg_cmd
                break;

            case CAN_TYPE_REQUEST_DATA:

                //get the memory offset
                offset = g_rMessage.msg_cmd;

                switch (g_rMessage.msg_read_write) {
                    case CAN_REQUEST_READ:

                        //WE MUST RECEIVE A REMOTE REQUEST...
                        if (g_rMessage.msg_remote == 1) {
                            can_read_data_flow_table_v2(offset,
                                    g_rMessage.msg_eeprom_ram,
                                    &g_rMessage.msg_data[0],
                                    MIN(g_rMessage.msg_data_length, 8));

                            g_rMessage.msg_remote = 0;

                            while (can_send_message(&g_rMessage)) {
                                ;
                            }

                        }

                        break;

                    case CAN_REQUEST_WRITE:
                        // We don't want to overwrite EEPROM table information
                        // The message must not be a remote request
                        if (g_rMessage.msg_remote == 0) {
                            can_write_data_flow_table_v2(offset,
                                    g_rMessage.msg_eeprom_ram,
                                    &g_rMessage.msg_data[0],
                                    MIN(g_rMessage.msg_data_length, 8));
                        }
                        break;

                }//end sub-switch read_write
                break;
        }
        // processe the received message
        can_proc_message(&g_rMessage);
    }
}



//////////////////////////////////////////////////////////////////////
//   can_send_im_alive
//////////////////////////////////////////////////////////////////////
//
//   Description: Send a I'M Alive can message
//
//   Input: can_addr (Address read in the EEPROM at 0XFE)
//   Output: NONE
//   Input/Output: NONE
//   Returned value: NONE
//
//////////////////////////////////////////////////////////////////////

void can_send_im_alive(unsigned char can_addr) {
    CAN_MESSAGE msg;
    int i = 0;

    msg.msg_priority = 0x00;
    msg.msg_type = CAN_TYPE_EVENTS;
    msg.msg_cmd = CAN_EVENTS_CMD_ALIVE;
    msg.msg_dest = can_addr;
    msg.msg_eeprom_ram = CAN_REQUEST_EEPROM;
    msg.msg_read_write = CAN_REQUEST_READ;
    msg.msg_data_length = 8;

    //Protocol version 2
    for (i = 0; i < 8; i++) {
        msg.msg_data[i] = can_read_eeprom(i);
    }

    //make sure we are returning the right can_addr
    msg.msg_data[2] = can_addr;

    msg.msg_remote = 0;
    can_send_message(&msg);
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

unsigned char can_read_data_flow_table_v2(unsigned int offset, unsigned char mem_type, unsigned char *buffer, unsigned int size) {
    unsigned int i = 0;
    unsigned char success = 1;

    switch (mem_type) {
        case CAN_REQUEST_RAM:
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

        case CAN_REQUEST_EEPROM:
            if (offset + size <= DATA_FLOW_TABLE_SIZE) {
                INTCONbits.GIEH = 0; //disable interrupts
                for (i = offset; i < (offset + size); i++) {
                    buffer[i - offset] = can_read_eeprom(i);
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

unsigned char can_write_data_flow_table_v2(unsigned int offset, unsigned char mem_type, unsigned char *buffer, unsigned int size) {

    unsigned int i = 0;
    unsigned char success = 1;

    switch (mem_type) {
        case CAN_REQUEST_RAM:
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


        case CAN_REQUEST_EEPROM:
            //must protect firt 8 bytes.
            if ((offset + size <= DATA_FLOW_TABLE_SIZE) && (offset >= 8)) {
                INTCONbits.GIEH = 0; //disable interrupts
                for (i = offset; i < (offset + size); i++) {
                    can_write_eeprom(i, buffer[i - offset]);
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
    _asm
    TBLRDPOSTINC
    _endasm
    return TABLAT;
}

void can_read_boot_config(BootConfig *config) {
    unsigned int devid = 0;

    if (config) {
        config->module_state = can_read_eeprom(0);
        config->project_id = can_read_eeprom(1);
        config->module_id = can_read_eeprom(2);
        config->code_version = can_read_eeprom(3);
        config->table_version = can_read_eeprom(4);
        config->boot_delay = can_read_eeprom(5);

        //read devid
        TBLPTR = DEVID_BASE_ADDRESS;
        config->devid_low = table_read();
        config->devid_high = table_read();
    }
}

void can_write_boot_config(BootConfig *config) {
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
            can_write_eeprom(i, data[i]);
        }
    }
}

BootConfig* can_get_boot_config(void) {
    return &g_BootConfig;
}




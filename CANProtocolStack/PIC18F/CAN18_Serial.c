#include "CAN18_Serial.h"
#include "CAN18_Device.h"
#include "CAN18_Shared.h"
#include "CAN18_Utils.h"
#include <usart.h>
#include <delays.h>




//CANRxMessageBuffer g_RxMessageBuffer[RX_BUFFER_SIZE];
//CANTxMessageBuffer g_TxMessageBuffer[TX_BUFFER_SIZE];


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
        putcUSART(buf.messageBytes[i]);
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
    getsUSART((char*)&buf.messageBytes[0],sizeof(CANRxMessageBuffer));

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









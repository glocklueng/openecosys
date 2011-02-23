#ifndef _CAN18_SERIAL_H_
#define _CAN18_SERIAL_H_

#include "CAN18_Common.h"

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



#define RX_BUFFER_SIZE 5
#define TX_BUFFER_SIZE 5

//Serial functions


#endif



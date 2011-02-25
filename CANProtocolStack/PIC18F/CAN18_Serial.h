#ifndef _CAN18_SERIAL_H_
#define _CAN18_SERIAL_H_

#include "C:\Users\Meka-Intern\Dropbox\Prog\svn_openecosys\PIC18F\CAN18_Common.h"

#define START_BYTE 0xF2

typedef union  
{
	struct {	
		unsigned char start_byte;
		unsigned char pri_boot_rtr;
		unsigned char type;
		unsigned char cmd;
		unsigned char dest;
		unsigned char data_length_iface;
		unsigned char data[8];
		unsigned char checksum;
	};
	
	unsigned char messageBytes[15];
} CANSerialMessage;


#define RX_BUFFER_SIZE (10 * sizeof(CANSerialMessage))

void serial_usart_interrupt_handler(void);
unsigned char serial_calculate_checksum(const CANSerialMessage *message);


#endif



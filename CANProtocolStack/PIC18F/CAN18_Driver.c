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

// Library call
#include "CAN18_Driver.h"
#include "CAN18_Device.h"
#include <delays.h>


void  can_set_baud(void);
void  can_set_mode(unsigned char mode);

// Internal enums
enum CAN_OP_MODE {CAN_OP_CONFIG=4, CAN_OP_LISTEN=3, CAN_OP_LOOPBACK=2, CAN_OP_DISABLE=1, CAN_OP_NORMAL=0};
enum CAN_WIN_ADDRESS {CAN_WIN_RX0=0, CAN_WIN_RX1=5, CAN_WIN_TX0=4, CAN_WIN_TX1=3, CAN_WIN_TX2=2};
enum CAN_int_CODE {CAN_int_WAKEUP=7, CAN_int_RX0=6, CAN_int_RX1=5, CAN_int_TX0=4, CAN_int_TX1=3, CAN_int_TX2=2, CAN_int_ERROR=1, CAN_int_NO=0};
enum CAN_RX_MODE {CAN_RX_ALL=3, CAN_RX_EXT=2, CAN_RX_STD=1, CAN_RX_VALID=0};


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
   unsigned char TXBxSIDH = 0;
   unsigned char TXBxSIDL = 0;
   unsigned char TXBxEIDH = 0;
   unsigned char TXBxEIDL = 0;
   
   if (message) {

	   // TXBxSIDH: priority + type
	   TXBxSIDH = (message->msg_priority << 5) | (message->msg_type >>3);
	
	   // TXBxSIDL: extended frame + type + eeprom_ram + read_write
	   TXBxSIDL = (message->msg_type << 5) | 0x08;
	   TXBxSIDL |= ((message->msg_eeprom_ram & 0x01) << 1) | (message->msg_read_write & 0x01);
	
	   // TXBxEID8: command
	   TXBxEIDH = (message->msg_cmd);
	
	   // TXBxEID0: destination
	   TXBxEIDL = (message->msg_dest);
	
	   //find emtpy transmitter
	   //map access bank addresses to empty transmitter
	   if (!TXB0CONbits.TXREQ) {
	      
		  CANCONbits.WIN0 = CAN_WIN_TX0;
		  CANCONbits.WIN1 = CAN_WIN_TX0 >> 1;
	      CANCONbits.WIN2 = CAN_WIN_TX0 >> 2;
	      
	      //priority (highest priority)
		  TXB0CONbits.TXPRI0 = 1; 
		  TXB0CONbits.TXPRI1 = 1;
		  
		  //datalength + remote request
		  TXB0DLC = (message->msg_data_length | (message->msg_remote << 6));
		        
		  //set CAN ID
		  TXB0SIDH = TXBxSIDH;
		  TXB0SIDL = TXBxSIDL;
		  TXB0EIDH = TXBxEIDH;
		  TXB0EIDL = TXBxEIDL;
		   
	      //copy 8 CAN data bytes
		  TXB0D0 = message->msg_data[0]; 
		  TXB0D1 = message->msg_data[1]; 
		  TXB0D2 = message->msg_data[2]; 
		  TXB0D3 = message->msg_data[3]; 
		  TXB0D4 = message->msg_data[4]; 
		  TXB0D5 = message->msg_data[5]; 
		  TXB0D6 = message->msg_data[6]; 
		  TXB0D7 = message->msg_data[7]; 
	
	   	  //enable transmission
	      TXB0CONbits.TXREQ=1;
	   }
	   else if (!TXB1CONbits.TXREQ) {
	      CANCONbits.WIN0 = CAN_WIN_TX1;
		  CANCONbits.WIN1 = CAN_WIN_TX1 >> 1;
	      CANCONbits.WIN2 = CAN_WIN_TX1 >> 2;
	
	      //priority (highest priority)
		  TXB1CONbits.TXPRI0 = 1; 
		  TXB1CONbits.TXPRI1 = 1;
		  
		  //datalength + remote request
		  TXB1DLC = (message->msg_data_length | (message->msg_remote << 6));
		        
		  //set CAN ID
		  TXB1SIDH = TXBxSIDH;
		  TXB1SIDL = TXBxSIDL;
		  TXB1EIDH = TXBxEIDH;
		  TXB1EIDL = TXBxEIDL;
		   
	      //copy 8 CAN data bytes
		  TXB1D0 = message->msg_data[0]; 
		  TXB1D1 = message->msg_data[1]; 
		  TXB1D2 = message->msg_data[2]; 
		  TXB1D3 = message->msg_data[3]; 
		  TXB1D4 = message->msg_data[4]; 
		  TXB1D5 = message->msg_data[5]; 
		  TXB1D6 = message->msg_data[6]; 
		  TXB1D7 = message->msg_data[7]; 
	
		  //enable transmission
	      TXB1CONbits.TXREQ=1;
	   }
	   else if (!TXB2CONbits.TXREQ) {
	      CANCONbits.WIN0 = CAN_WIN_TX2;
		  CANCONbits.WIN1 = CAN_WIN_TX2 >> 1;
	      CANCONbits.WIN2 = CAN_WIN_TX2 >> 2;
	
	      //priority (highest priority)
		  TXB2CONbits.TXPRI0 = 1; 
		  TXB2CONbits.TXPRI1 = 1;
		  
		  //datalength + remote request
		  TXB2DLC = (message->msg_data_length | (message->msg_remote << 6));
		        
		  //set CAN ID
		  TXB2SIDH = TXBxSIDH;
		  TXB2SIDL = TXBxSIDL;
		  TXB2EIDH = TXBxEIDH;
		  TXB2EIDL = TXBxEIDL;
		   
	      //copy 8 CAN data bytes
		  TXB2D0 = message->msg_data[0]; 
		  TXB2D1 = message->msg_data[1]; 
		  TXB2D2 = message->msg_data[2]; 
		  TXB2D3 = message->msg_data[3]; 
		  TXB2D4 = message->msg_data[4]; 
		  TXB2D5 = message->msg_data[5]; 
		  TXB2D6 = message->msg_data[6]; 
		  TXB2D7 = message->msg_data[7]; 
	
	 	   //enable transmission
	       TXB2CONbits.TXREQ=1;
		}
		else {
			return(1);
		}
	
		CANCONbits.WIN0 = CAN_WIN_RX0;
		CANCONbits.WIN1 = CAN_WIN_RX0 >> 1;
		CANCONbits.WIN2 = CAN_WIN_RX0 >> 2;
   
   		return 0;
   	}
   	else {
		return 1;
	}
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
	unsigned char retval = 0;

	if (message) {
	    if (RXB0CONbits.RXFUL) {

        	CANCONbits.WIN0=CAN_WIN_RX0;
			CANCONbits.WIN1=CAN_WIN_RX0 >> 1;
			CANCONbits.WIN2=CAN_WIN_RX0 >> 2;
        
			//clear interrupt flag
			PIR3bits.RXB0IF = 0;
               
			//clear overflow bit
        	COMSTATbits.RXB0OVFL = 0;
			
			//data length
			message->msg_data_length = RXB0DLC & 0x0F;        

			//RTR
			message->msg_remote = RXB0DLC >> 6;
    	
			//RXBxSIDH
      		message->msg_priority = RXB0SIDH >> 5;
      		message->msg_type = RXB0SIDH << 3;

      		// RXBxSIDL
      		message->msg_type |= RXB0SIDL >> 5;
      		message->msg_read_write = RXB0SIDL & 0x01;
            message->msg_eeprom_ram = (RXB0SIDL & 0x02) >> 1;

      		// RXBxEIDH
      		message->msg_cmd  = RXB0EIDH;

      		// RXBxEIDL
      	    message->msg_dest = RXB0EIDL;

		
			//COPY DATA
			message->msg_data[0] = RXB0D0;
			message->msg_data[1] = RXB0D1;
			message->msg_data[2] = RXB0D2;
			message->msg_data[3] = RXB0D3;
			message->msg_data[4] = RXB0D4;
			message->msg_data[5] = RXB0D5;
			message->msg_data[6] = RXB0D6;
			message->msg_data[7] = RXB0D7;
		
			//done with RXB0
			RXB0CONbits.RXFUL=0;

			retval = 1;
	    }
	    else if ( RXB1CONbits.RXFUL )
	    {
	        
			CANCONbits.WIN0=CAN_WIN_RX1;
			CANCONbits.WIN1=CAN_WIN_RX1 >> 1;
			CANCONbits.WIN2=CAN_WIN_RX1 >> 2;
	
			//clear interrupt flag
			PIR3bits.RXB1IF = 0;
	
			//clear overflow bit
	       	COMSTATbits.RXB1OVFL = 0;
			
			//data length
			message->msg_data_length = RXB1DLC & 0x0F;        
	
			//RTR
			message->msg_remote = RXB1DLC >> 6;
	   	
			//RXBxSIDH
	   		message->msg_priority = RXB1SIDH >> 5;
	   		message->msg_type = RXB1SIDH << 3;
	
	   		// RXBxSIDL
	   		message->msg_type |= RXB1SIDL >> 5;
	   		message->msg_read_write = RXB1SIDL & 0x01;
	        message->msg_eeprom_ram = (RXB1SIDL & 0x02) >> 1;
	
	   		// RXBxEIDH
	   		message->msg_cmd  = RXB1EIDH;
	
	   		// RXBxEIDL
	   	    message->msg_dest = RXB1EIDL;
	
		
			//COPY DATA
			message->msg_data[0] = RXB1D0;
			message->msg_data[1] = RXB1D1;
			message->msg_data[2] = RXB1D2;
			message->msg_data[3] = RXB1D3;
			message->msg_data[4] = RXB1D4;
			message->msg_data[5] = RXB1D5;
			message->msg_data[6] = RXB1D6;
			message->msg_data[7] = RXB1D7;
	
	    
			RXB1CONbits.RXFUL=0;
	
			retval = 1;
	    }
	    else {
	      retval = 0;
	    }
	}


 	return retval;
}

//////////////////////////////////////////////////////////////////////
//   can_apply_accept_mask
//////////////////////////////////////////////////////////////////////
//
//   Description: Sets the MCP2510 in configuration mode
//                Updates the specified mask and configures it to its
//                previous mode
//
//   Input: object_id specifying the mask number
//   Output: NONE
//   Input/Output: mask
//   Returned value: NONE
//
//////////////////////////////////////////////////////////////////////
void can_apply_accept_mask(CAN_MASK *mask, unsigned char mask_id)
{   
   unsigned char TXBxSIDH = 0;
   unsigned char TXBxSIDL = 0;
   unsigned char TXBxEIDH = 0;
   unsigned char TXBxEIDL = 0;
    
   if (mask) {

	   // TXBxSIDH: priority + type
	   TXBxSIDH = (mask->mask_priority << 5) | (mask->mask_type >>3);
	
	   // TXBxSIDL: extended frame + type
	   TXBxSIDL = (mask->mask_type << 5);
	
	   // TXBxEID8: command
	   TXBxEIDH = (mask->mask_cmd);
	
	   // TXBxEID0: destination
	   TXBxEIDL = (mask->mask_dest);
	
	   switch(mask_id) {
	      case 0:
			RXM0SIDH = TXBxSIDH;
			RXM0SIDL = TXBxSIDL;
			RXM0EIDH = TXBxEIDH;
			RXM0EIDL = TXBxEIDL;
	      break;
	
	      case 1:
			RXM1SIDH = TXBxSIDH;
			RXM1SIDL = TXBxSIDL;
			RXM1EIDH = TXBxEIDH;
			RXM1EIDL = TXBxEIDL;
	      break;
	   }
   }
}

//////////////////////////////////////////////////////////////////////
//   can_apply_filter
//////////////////////////////////////////////////////////////////////
//
//   Description: Sets the MCP2510 in configuration mode
//                Updates the specified filter and configures it to its
//                previous mode
//
//   Input: object_id specifying the filter number
//   Output: NONE
//   Input/Output: filter
//   Returned value: NONE
//
//////////////////////////////////////////////////////////////////////
	void can_apply_filter(CAN_FILTER *filter, unsigned char filter_id)
	{   
	   unsigned char TXBxSIDH = 0;
	   unsigned char TXBxSIDL = 0;
	   unsigned char TXBxEIDH = 0;
	   unsigned char TXBxEIDL = 0;
	   
	   if (filter) {
		   // TXBxSIDH: priority + type
		   TXBxSIDH = (filter->filter_priority << 5) | (filter->filter_type >>3);   
			TXBxSIDH = (filter->filter_priority << 5) | (filter->filter_type >>3);
		   TXBxSIDH = (filter->filter_priority << 5) | (filter->filter_type >>3);
		   // TXBxSIDL: extended frame + type
		   TXBxSIDL = (filter->filter_type << 5) | 0x08;
		
		   // TXBxEID8: command
		   TXBxEIDH = (filter->filter_cmd);
		
		   // TXBxEID0: destination
		   TXBxEIDL = (filter->filter_dest);
		
		   switch(filter_id) {
		      case 0:
				RXF0SIDH = TXBxSIDH;
				RXF0SIDL = TXBxSIDL;
				RXF0EIDH = TXBxEIDH;
				RXF0EIDL = TXBxEIDL;
		      break;
		
		      case 1:
				RXF1SIDH = TXBxSIDH;
				RXF1SIDL = TXBxSIDL;
				RXF1EIDH = TXBxEIDH;
				RXF1EIDL = TXBxEIDL;
		      break;
		
		      case 2:
				RXF2SIDH = TXBxSIDH;
				RXF3SIDL = TXBxSIDL;
				RXF4EIDH = TXBxEIDH;
				RXF5EIDL = TXBxEIDL;
		      break;
		
		      case 3:
				RXF3SIDH = TXBxSIDH;
				RXF3SIDL = TXBxSIDL;
				RXF3EIDH = TXBxEIDH;
				RXF3EIDL = TXBxEIDL;
		      break;
		
		      case 4:
				RXF4SIDH = TXBxSIDH;
				RXF4SIDL = TXBxSIDL;
				RXF4EIDH = TXBxEIDH;
				RXF4EIDL = TXBxEIDL;
		      break;
		
		      case 5:
				RXF5SIDH = TXBxSIDH;
				RXF5SIDL = TXBxSIDL;
				RXF5EIDH = TXBxEIDH;
				RXF5EIDL = TXBxEIDL;
		      break;
		   }
	}
}












////////////////////////////////////////////////////////////////////////
//
// can_init()
//
// Initializes PIC18xxx8 CAN peripheral.  Sets the RX filter and masks so the
// CAN peripheral will receive all incoming IDs.  Configures both RX buffers
// to only accept valid valid messages (as opposed to all messages, or all
// extended message, or all standard messages).  Also sets the tri-state
// setting of B2 to output, and B3 to input (apparently the CAN peripheral
// doesn't keep track of this)
//
// The constants (CAN_USE_RX_DOUBLE_BUFFER, CAN_ENABLE_DRIVE_HIGH,
// CAN_ENABLE_CAN_CAPTURE) are given a default define in the can-18xxx8.h file.
// These default values can be overwritten in the main code, but most
// applications will be fine with these defaults.
//
//////////////////////////////////////////////////////////////////////////////
void can_init (CAN_FILTER *filter, CAN_MASK *mask) {
   unsigned char i = 0;

   //PIN B3 (RX) is in, B2 (TX) is out
   TRISBbits.TRISB3 = 1;
   TRISBbits.TRISB2 = 0;

   //Get in config mode for now
   can_set_mode(CAN_OP_CONFIG);

   //make sure to possess a filter with dest = 0xFF
   filter[5].filter_dest |= 0xFF;

   //Set Filter
   for(i=0;i<6;i++){
      can_apply_filter(&filter[i],i);
   }

   //make sure that mask[1].dest = at least 0xFF
   mask[1].mask_dest |= 0xFF;

   //Set MASK
   for(i=0;i<2;i++){
      can_apply_accept_mask(&mask[i],i);
   }

   //Set Bit rate
   can_set_baud();

   //Set IO
   RXB0CON=0;

   RXB0CONbits.RXM0=CAN_RX_VALID >> 1;
   RXB0CONbits.RXM1=CAN_RX_VALID;
   RXB0CONbits.RXB0DBEN = 1; //Use Double buffer
   RXB1CON=RXB0CON;

   //CIOCONbits.ENDRHI=CAN_ENABLE_DRIVE_HIGH;
   //CIOCONbits.CANCAP=CAN_ENABLE_CAN_CAPTURE;
   CIOCON = 0b00100000; //CAN IO control

   //Get to normal again
   can_set_mode(CAN_OP_NORMAL);  
}

////////////////////////////////////////////////////////////////////////
//
// can_set_baud()
//
// Configures the baud rate control registers.  All the defines here
// are defaulted in the can-18xxx8.h file.  These defaults can, and
// probably should, be overwritten in the main code.
//
// Current defaults are set to work with Microchip's MCP250xxx CAN
// Developers Kit if this PIC is running at 20Mhz.
//
////////////////////////////////////////////////////////////////////////
void can_set_baud(void) {

	//1MBPS @ 10Mips
	BRGCON1 = 0b00000000;
	BRGCON2 = 0b10111010;
	BRGCON3 = 0b00000111;
}

void can_set_mode(unsigned char mode) {

   CANCONbits.REQOP0 = mode;
   CANCONbits.REQOP1 = mode >> 1;
   CANCONbits.REQOP2 = mode >> 2;
	
   
   //CIOCON = 0b00100000; //CAN IO control

   while((CANSTAT >> 5) != mode );
}

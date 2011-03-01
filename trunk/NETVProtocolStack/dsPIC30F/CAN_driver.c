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

#include <can.h>
#include "CAN_memory.h"
#include "CAN_utils.h"
#include "CAN_Device.h"
#include "CAN_driver.h"
#include "CAN_shared.h"

//Prototypes
void can_write_eeprom(unsigned int index, unsigned int data);
unsigned int can_read_eeprom(unsigned int index);
unsigned char can_read_data_flow_table_v2(unsigned int offset, unsigned char mem_type, unsigned char *buffer, unsigned int size);
unsigned char can_write_data_flow_table_v2(unsigned int offset, unsigned char mem_type, unsigned char *buffer, unsigned int size);
unsigned char can_eeprom_hit(void);

//GLOBAL VARIABLES TO BE USED IN ALL SOURCE FILES
CAN_MESSAGE g_rMessage = {0};    // all downloaded messages from PIC
                                 // are written here

//GLOBAL BOOT CONFIGURATION USED FOR THIS DEVICE
BootConfig g_BootConfig;

//Quiet mode to avoid transmission on the bus
unsigned char g_MSG_SILENCE = 0;

//MAXIMUM ALLOWED SIZE IS 256 BYTES
unsigned char* const DATA_FLOW_TABLE = (unsigned char*) &g_globalCANVariables;


/*******************************************************************
READ EEPROM
 *******************************************************************/
unsigned int can_read_eeprom(unsigned int index) 
{ 
	return  ReadMem(EEPROM_BASE_ADDRESS_HIGH,EEPROM_BASE_ADDRESS_LOW + (index << 1));
} 

/*******************************************************************
WRITE EEPROM
 *******************************************************************/
void can_write_eeprom(unsigned int index, unsigned int data) 
{ 	
	WriteMem(EEPROM_BASE_ADDRESS_HIGH,EEPROM_BASE_ADDRESS_LOW + (index << 1),&data,1);
} 


/* LABORIUS SECTION ########################################################################################### */

//////////////////////////////////////////////////////////////////////
//   can_send_frame
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
char can_send_frame(CAN_MESSAGE *message)
{
	unsigned long caneid = 0;	
	unsigned char bufnum = 0;

	if (message) //pointer verification
	{			
		//transmissions allowed ?
		if (g_MSG_SILENCE == 0) 
		{

			//Extract eid from message
	
			//priority
			caneid |= (((unsigned long)message->msg_priority << 26) & 0x1C000000);
			//type
			caneid  |= (((unsigned long)message->msg_type << 18) & 0x03FC0000);
			//boot
			caneid  |= (((unsigned long)(((message->msg_eeprom_ram & 0x01) << 1) | (message->msg_read_write & 0x01)) << 16) & 0x00030000);
			//cmd
			caneid  |= (((unsigned long) message->msg_cmd << 8) & 0x0000FF00);
			//dest
			caneid  |= (((unsigned long) message->msg_dest) & 0x000000FF);
	
			
			if (CAN1IsTXReady(0)) //Transmit buffer 0 ready?
			{
				bufnum = 0;
			}
			else if (CAN1IsTXReady(1)) //Transmit buffer 1 ready?
			{
				bufnum = 1;
			}
			else if (CAN1IsTXReady(2)) //Transmit buffer 2 ready?
			{
				bufnum = 2;
			}
			else //No transmit buffer available
			{
				//RETURN ERROR CODE
				return -1;
			}

			//TRANSMIT DATA
			if (message->msg_remote) //SENDING REMOTE FRAME
			{
				//force data length to zero
				CAN1SendMessage(CAN_TX_SID(caneid >> 18) & CAN_TX_EID_EN,
								(CAN_TX_EID(caneid)) & (CAN_REM_TX_REQ),
								message->msg_data, 0, bufnum);
			}
			else //SENDING NORMAL FRAME
			{
				CAN1SendMessage(CAN_TX_SID(caneid >> 18) & CAN_TX_EID_EN,
					(CAN_TX_EID(caneid)) & (CAN_NOR_TX_REQ),
					message->msg_data, message->msg_data_length, bufnum);
			}

			//RETURN SUCCESS CODE
			return 0;

		} //! MESSAGE SILENCE
		else {
			//RETURN SUCCESS CODE FOR SILENT MODE
			return 0;
		} //MESSAGE SILENCE
	} //IF MESSAGE

	//SOMETHING IS WRONG
	//RETURN ERROR CODE
	return -1;
}

//////////////////////////////////////////////////////////////////////
//   can_recv_frame
//////////////////////////////////////////////////////////////////////
//
//   Description: Extract RX buffer message and put it in a message
//                structure.
//
//   Input: object_id specifying the RX buffer to use
//   Output: NONE
//   Input/Output: message
//   Returned value: NONE
//
//////////////////////////////////////////////////////////////////////
char can_recv_frame(CAN_MESSAGE *message)
{
	unsigned char retval = 0;

	if (message)
	{
		//Provided dsPIC library does not offer enough functionalities
		//for reading messages. Doing it by hand.
		if(C1RX0CONbits.RXFUL) //READ RX0 FRAME
		{

			//READING EID
			//PRIORITY
			message->msg_priority = (CAN1.cxrx0sid >> 10) & 0x0007;

			//TYPE
			message->msg_type = (CAN1.cxrx0sid >> 2) & 0x00FF;

			//BOOT
			message->msg_eeprom_ram = (CAN1.cxrx0eid >> 11) & 0x0001; 
			message->msg_read_write = (CAN1.cxrx0eid >> 10) & 0x0001;

			//CMD
			message->msg_cmd = (CAN1.cxrx0eid >> 2) & 0x00FF; 

			//DEST
			message->msg_dest = (((CAN1.cxrx0eid << 6) & 0x00C0) | ((CAN1.cxrx0dlc >> 10) & 0x003F ) )& 0x00FF;


			//MESSAGE REMOTE ?
			message->msg_remote = (CAN1.cxrx0dlc >> 9) & 0x0001;

			//READING DATA
			message->msg_data_length = CAN1.cxrx0dlc & 0x000F;
			message->msg_data[0] = CAN1.cxrx0b1;
			message->msg_data[1] = CAN1.cxrx0b1 >> 8;
			message->msg_data[2] = CAN1.cxrx0b2;
			message->msg_data[3] = CAN1.cxrx0b2 >> 8;
			message->msg_data[4] = CAN1.cxrx0b3;
			message->msg_data[5] = CAN1.cxrx0b3 >> 8;
			message->msg_data[6] = CAN1.cxrx0b4;
			message->msg_data[7] = CAN1.cxrx0b4 >> 8;
			
			//FILTER HIT
			message->msg_filter_hit = CAN1.cxrx0con & 0x0001;
			
			//CLEAR OVERFLOW
			message->msg_overflow = C1INTFbits.RX0OVR;
			C1INTFbits.RX0OVR = 0;

			//DONE WITH THIS FRAME
			C1RX0CONbits.RXFUL = 0;

			retval = 1;
		}
		else if (C1RX1CONbits.RXFUL) //READ RX1 FRAME
		{
			//READING EID
			//PRIORITY
			message->msg_priority = (CAN1.cxrx1sid >> 10) & 0x0007;

			//TYPE
			message->msg_type = (CAN1.cxrx1sid >> 2) & 0x00FF;

			//BOOT
			message->msg_eeprom_ram = (CAN1.cxrx1eid >> 11) & 0x0001; 
			message->msg_read_write = (CAN1.cxrx1eid >> 10) & 0x0001;

			//CMD
			message->msg_cmd = (CAN1.cxrx1eid >> 2) & 0x00FF; 

			//DEST
			message->msg_dest = (((CAN1.cxrx1eid << 6) & 0x00C0) | ((CAN1.cxrx1dlc >> 10) & 0x003F ) )& 0x00FF;


			//MESSAGE REMOTE ?
			message->msg_remote = (CAN1.cxrx1dlc >> 9) & 0x0001;

			//READING DATA
			message->msg_data_length = CAN1.cxrx1dlc & 0x000F;
			message->msg_data[0] = CAN1.cxrx1b1;
			message->msg_data[1] = CAN1.cxrx1b1 >> 8;
			message->msg_data[2] = CAN1.cxrx1b2;
			message->msg_data[3] = CAN1.cxrx1b2 >> 8;
			message->msg_data[4] = CAN1.cxrx1b3;
			message->msg_data[5] = CAN1.cxrx1b3 >> 8;
			message->msg_data[6] = CAN1.cxrx1b4;
			message->msg_data[7] = CAN1.cxrx1b4 >> 8;
			
			//FILTER HIT
			message->msg_filter_hit = CAN1.cxrx1con & 0x0007;

			//CLEAR OVERFLOW
			message->msg_overflow = C1INTFbits.RX1OVR;
			C1INTFbits.RX1OVR = 0;
			
			//DONE WITH THIS FRAME
			C1RX1CONbits.RXFUL = 0;

			retval = 1;
		}
	} //VALID MESSAGE PTR
	else
	{
		retval = -1;
	} //INVALID MESSAGE PTR
	

	return retval;
}

//////////////////////////////////////////////////////////////////////
//   can_apply_accept_mask
//////////////////////////////////////////////////////////////////////
//
//   Description: Updates the specified mask. Go in config mode and return to the
//                previous mode after the mask is configured.
//
//   Input: object_id specifying the mask number
//   Output: NONE
//   Input/Output: mask
//   Returned value: NONE
//
//////////////////////////////////////////////////////////////////////
void can_apply_accept_mask(CAN_MASK *mask, unsigned char mask_id)
{   
	unsigned long caneid = 0;

	if (mask && C1CTRLbits.REQOP == 0x04) //mask valid and configuration mode
	{
		//SETUP MASK
		if (mask_id < 2)
		{
			//priority
			caneid |= (((unsigned long)mask->mask_priority << 26) & 0x1C000000);
			//type
			caneid  |= (((unsigned long)mask->mask_type << 18) & 0x03FC0000);
			//cmd
			caneid  |= (((unsigned long) mask->mask_cmd << 8) & 0x0000FF00);
			//dest
			caneid  |= (((unsigned long) mask->mask_dest) & 0x000000FF);

			CAN1SetMask(mask_id, CAN_MASK_SID(caneid >> 18) & CAN_MATCH_FILTER_TYPE, CAN_MASK_EID(caneid));
		}
	}
}

//////////////////////////////////////////////////////////////////////
//   can_apply_filter
//////////////////////////////////////////////////////////////////////
//
//   Description: Updates the specified filter. Go in config mode and return to the
//                previous mode after the mask is configured.
//
//   Input: object_id specifying the filter number
//   Output: NONE
//   Input/Output: filter
//   Returned value: NONE
//
//////////////////////////////////////////////////////////////////////
void can_apply_filter(CAN_FILTER *filter, unsigned char filter_id)
{   
	unsigned long caneid = 0;

	//Filter must be valid and we need to be in configuration mode
	if (filter && C1CTRLbits.REQOP == 0x04) 
	{
		//SETUP FILTER
		if (filter_id < 6)
		{
			//priority
			caneid |= (((unsigned long)filter->filter_priority << 26) & 0x1C000000);
			//type
			caneid  |= (((unsigned long)filter->filter_type << 18) & 0x03FC0000);
			//cmd
			caneid  |= (((unsigned long) filter->filter_cmd << 8) & 0x0000FF00);
			//dest
			caneid  |= (((unsigned long) filter->filter_dest) & 0x000000FF);

			CAN1SetFilter(filter_id, CAN_FILTER_SID(caneid >> 18) & CAN_RX_EID_EN, CAN_FILTER_EID(caneid));
		}
	}
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
void can_transceiver(unsigned char can_addr)
{    
   unsigned char call_proc_message = 1;
   unsigned int offset = 0;

   while(can_recv_frame(&g_rMessage)) {
      //Analyse for boot mode and I'm alive signal
      switch(g_rMessage.msg_type){
      case CAN_TYPE_EMERGENCY:

         switch(g_rMessage.msg_cmd){
            case CAN_EMERGENCY_CMD_PROGRAM:
				if(g_rMessage.msg_dest == can_addr){

					//indicate to the bootloader that we are gonna program				
					call_proc_message = 0;
	
					g_BootConfig.module_state = BOOT_IDLE;
					can_write_boot_config(&g_BootConfig);
					
					//reset!
					RESET();
				}
               break;
         }//end switch msg_cmd

         break;

      case CAN_TYPE_EVENTS:
		//React only to your ID and the General ID but not to other addr.
		if((g_rMessage.msg_dest == can_addr) || (g_rMessage.msg_dest ==0xFF)){
			switch(g_rMessage.msg_cmd){
			   case CAN_EVENTS_CMD_ALIVE:
				    //Send i'm alive
				    can_send_im_alive(can_addr,&g_BootConfig);
					call_proc_message = 0;
				break;
				case CAN_EVENTS_CMD_SILENCE:
					//0 = Driver will send all msg ; 1 = Driver will not send msg except for the IM_ALIVE signal
					g_MSG_SILENCE = g_rMessage.msg_data[0];
					call_proc_message = 0;
				break;
			}//end switch msg_cmd
		}
		break;

      case CAN_TYPE_REQUEST_DATA:
		if(g_rMessage.msg_dest == can_addr){

			//Calculating offset (modified procotol)
			offset = g_rMessage.msg_priority;
			offset = (offset << 8);
			offset |= g_rMessage.msg_cmd;

	         switch(g_rMessage.msg_read_write){
	            case CAN_REQUEST_READ:
				   //WE MUST RECEIVE A REMOTE REQUEST...
				   if (g_rMessage.msg_remote == 1)
				   {
		               can_read_data_flow_table_v2(offset, 
											   g_rMessage.msg_eeprom_ram,
											   &g_rMessage.msg_data[0],
											   MIN(g_rMessage.msg_data_length,8));
	
		               g_rMessage.msg_remote = 0;
	
		               while(can_send_frame(&g_rMessage)){;}

				   }
				   call_proc_message = 0;
                   
  
	               break;
	
	            case CAN_REQUEST_WRITE:
	               // We don't want to overwrite EEPROM table information
				   // The message must not be a remote request
	               if(g_rMessage.msg_remote == 0)
				   {
						   can_write_data_flow_table_v2(offset, 
						   g_rMessage.msg_eeprom_ram,
						   &g_rMessage.msg_data[0],
						   MIN(g_rMessage.msg_data_length,8));
                   }


	
				   call_proc_message = 0;
	               break;
	
	         }//end sub-switch read_write
		}//End if can_addr
		break;  
      }

      //processe the received message
	  if (call_proc_message) {
      	can_proc_message(&g_rMessage);
	  }
   }//while
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
void can_send_im_alive(unsigned char addr, BootConfig *config)
{
	CAN_MESSAGE msg;
	unsigned char old_silence = g_MSG_SILENCE;

	//Alive message must not be silenced
	g_MSG_SILENCE = 0;
  
	if (config)
	{
		msg.msg_priority = 0x07;
		msg.msg_type = CAN_TYPE_EVENTS;
		msg.msg_cmd = CAN_EVENTS_CMD_ALIVE;
		msg.msg_dest = addr;
		msg.msg_eeprom_ram = CAN_REQUEST_EEPROM;
		msg.msg_read_write = CAN_REQUEST_READ;
		msg.msg_data_length = 8;
		msg.msg_remote = 0;

		//FILL DATA
		msg.msg_data[0] = config->module_state;
		msg.msg_data[1] = config->project_id;
		msg.msg_data[2] = config->module_id;
		msg.msg_data[3] = config->code_version;
		msg.msg_data[4] = config->table_version;
		msg.msg_data[5] = config->boot_delay;
		msg.msg_data[6] = config->devid_low;
		msg.msg_data[7] = config->devid_high;

		//Activate all msg to be sent
		while(can_send_frame(&msg));
	}

	//Put back silence to the state it was
	g_MSG_SILENCE = old_silence;	
}

/* DATA FLOW TABLE FUNCTIONS ################################################################### */

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

unsigned char can_read_data_flow_table_v2(unsigned int offset, unsigned char mem_type, unsigned char *buffer, unsigned int size)
{	
   	unsigned int i = 0;
   	unsigned char success = 1;

	switch(mem_type)
	{
		case CAN_REQUEST_RAM:
			if (offset + size <= DATA_FLOW_TABLE_SIZE)
			{
				__asm__ volatile ("disi #0x3FFF");
				for (i = offset; i < (offset + size); i++)
				{
					buffer[i - offset] = DATA_FLOW_TABLE[i];
				}
				__asm__ volatile ("disi #0x000");
	
				success = 1;
			}
			else
			{
				//memory out of bound
				success = 0;
			}
	 	break;

	    case CAN_REQUEST_EEPROM:
			if (offset + size <= DATA_FLOW_TABLE_SIZE)
			{
				__asm__ volatile ("disi #0x3FFF");
				for (i = offset; i < (offset + size); i++)
				{
					buffer[i - offset] = can_read_eeprom(i);
				}
				__asm__ volatile ("disi #0x000");
	
				success = 1;
			}
			else
			{
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
unsigned char can_write_data_flow_table_v2(unsigned int offset,unsigned char mem_type, unsigned char *buffer, unsigned int size)
{

	unsigned int i = 0;
	unsigned char success = 1;
	
	switch(mem_type)
	{
		case CAN_REQUEST_RAM:
			if (offset + size <= DATA_FLOW_TABLE_SIZE)
			{
				__asm__ volatile ("disi #0x3FFF");
				for (i = offset; i < (offset + size); i++)
				{
					DATA_FLOW_TABLE[i] = buffer[i - offset];
				}
				__asm__ volatile ("disi #0x000");
	
				success = 1;
			}
			else
			{
				//memory out of bound
				success = 0;
			}
		break;
	
	
	  	case CAN_REQUEST_EEPROM:
			//must protect firt 8 bytes.
			if ((offset + size <= DATA_FLOW_TABLE_SIZE) && (offset >= 8))
			{
				__asm__ volatile ("disi #0x3FFF");
				for (i = offset; i < (offset + size); i++)
				{
					can_write_eeprom(i,buffer[i - offset]);
				}
				__asm__ volatile ("disi #0x000");
	
				success = 1;
			}
			else
			{
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



/* INTERNAL DRIVER SECTION ##################################################################### */

////////////////////////////////////////////////////////////////////////
//
// can_init()
//
// Initializes dsPIC30F CAN peripheral.  Sets the RX filter and masks so the
// CAN peripheral will receive all incoming IDs.  Configures both RX buffers
// to only accept valid valid messages (as opposed to all messages, or all
// extended message, or all standard messages).  Also sets the tri-state
// setting of F1 to output, and F0 to input (apparently the CAN peripheral
// doesn't keep track of this)
//
//
//////////////////////////////////////////////////////////////////////////////
void can_init (CAN_FILTER *filter, CAN_MASK *mask) {

	unsigned char i = 0;

	// Setup input and output pins
	TRISFbits.TRISF0 = 1; //CAN RX
	TRISFbits.TRISF1 = 0; //CAN TX

	// Set request for configuration mode
	// FCAN = FCY
	CAN1SetOperationMode(CAN_IDLE_CON &
	CAN_MASTERCLOCK_1 &
	CAN_REQ_OPERMODE_CONFIG &
	CAN_CAPTURE_DIS);

	// Wait until we are in the config mode
	while(C1CTRLbits.OPMODE != C1CTRLbits.REQOP);

	// Load configuration register
	CAN1Initialize(CAN_SYNC_JUMP_WIDTH1 &
		CAN_BAUD_PRE_SCALE(1),
		CAN_WAKEUP_BY_FILTER_DIS &
		CAN_PHASE_SEG2_TQ(3) &
		CAN_PHASE_SEG1_TQ(5) &
		CAN_PROPAGATIONTIME_SEG_TQ(1) &
		CAN_SEG2_FREE_PROG &
		CAN_SAMPLE1TIME);
	
	//WORKING SETUP
	//C1CFG1 = 0x0000; //BRP = 2 * (1) / FCY, SJW = 1 TQ
	//C1CFG2 = 0x02A0; //SEG2 = 3 TQ, SEG1 = 5TQ PRSEG = 1TQ, SEG2 FREELY PROG, SAMPLE ONCE

   	//make sure that mask[1].dest = at least 0xFF
   	mask[1].mask_dest = 0xFF;

   	//Set MASK
   	for(i=0;i<2;i++){
      	can_apply_accept_mask(&mask[i],i);
   	}

	//Setup filters
 	//make sure to possess a filter with dest = 0xFF
   	filter[2].filter_dest = 0xFF;

   	//Set Filter
   	for(i=0;i<6;i++){
      	can_apply_filter(&filter[i],i);
   	}


	//Transmit buffer configuration
	for (i = 0; i < 3; i++)
	{
		CAN1SetTXMode(i,
			CAN_TX_STOP_REQ &
			CAN_TX_PRIORITY_HIGH );
	}

	//Receive buffer configuration
	for (i = 0; i < 2; i++)
	{
		CAN1SetRXMode(i,
			CAN_RXFUL_CLEAR &
			CAN_BUF0_DBLBUFFER_EN);
	}

	/* Set request for Normal mode */
	CAN1SetOperationMode(CAN_IDLE_CON & CAN_CAPTURE_DIS &
	CAN_MASTERCLOCK_1 &
	CAN_REQ_OPERMODE_NOR);

	//WAIT until we are in normal mode
	while(C1CTRLbits.OPMODE != C1CTRLbits.REQOP);

	return;

}


void can_read_boot_config(BootConfig *config)
{
	unsigned int devid = 0;

	if (config)
	{
		config->module_state  = (unsigned char) ReadMem(EEPROM_BASE_ADDRESS_HIGH,EEPROM_BASE_ADDRESS_LOW);
		config->project_id  = (unsigned char) ReadMem(EEPROM_BASE_ADDRESS_HIGH,EEPROM_BASE_ADDRESS_LOW + 2);
		config->module_id  = (unsigned char) ReadMem(EEPROM_BASE_ADDRESS_HIGH,EEPROM_BASE_ADDRESS_LOW + 4);
		config->code_version  = (unsigned char) ReadMem(EEPROM_BASE_ADDRESS_HIGH,EEPROM_BASE_ADDRESS_LOW + 6);
		config->table_version = (unsigned char) ReadMem(EEPROM_BASE_ADDRESS_HIGH,EEPROM_BASE_ADDRESS_LOW + 8);
		config->boot_delay  = (unsigned char) ReadMem(EEPROM_BASE_ADDRESS_HIGH,EEPROM_BASE_ADDRESS_LOW + 10);

		//read devid
		devid = ReadMem(0xFF,0x0000);
		config->devid_low = devid & 0x00FF;
		config->devid_high = devid >> 8;

	}
}

void can_write_boot_config(BootConfig *config)
{
	if (config)
	{

		unsigned int data[16]; //first page of data
		unsigned int addrlow = 0;

		//READING MEMORY
		for (addrlow = 0; addrlow < 32; addrlow += 2)
		{
			data[addrlow >> 1] = ReadMem(EEPROM_BASE_ADDRESS_HIGH,EEPROM_BASE_ADDRESS_LOW + addrlow);
		}

		data[0] = config->module_state;
		data[1] = config->project_id;
		data[2] = config->module_id;
		data[3] = config->code_version;
		data[4] = config->table_version;
		data[5] = config->boot_delay;
		//DEVID DOES NOT NEED TO BE WRITTEN!

		//WRITING BACK PAGE
		WriteMem(EEPROM_BASE_ADDRESS_HIGH,EEPROM_BASE_ADDRESS_LOW,data,16);

	}
}

BootConfig* can_get_boot_config()
{
	return &g_BootConfig;
}




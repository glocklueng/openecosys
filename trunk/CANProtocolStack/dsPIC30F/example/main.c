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

#include "CAN_device.h"
#include "CAN_driver.h"
#include "CAN_shared.h"
#include "CAN_memory.h"

#include <string.h>



void init_default_variables()
{
	memset(&g_globalCANVariables, 0, sizeof(GlobalCANVariables));
}



/************************************************************************
void proc_message(CAN_MESSAGE *message)

CAN MESSAGES HANDLING HERE!

**************************************************************************/
void can_proc_message(CAN_MESSAGE *message)
{
	//Handle application messages here


}


/************************************************************************
	MAIN PROGRAM
**************************************************************************/
int main(void)
{	
	CAN_MASK mask_in[2];
	CAN_FILTER filter_in[6];
	BootConfig *bootConfig = NULL;
	unsigned char Can_Addr = 0;
	unsigned int i = 0;


	//reading boot config and device configuration
	//MUST BE DONE BEFORE INITIALIZING CAN MODULE
	bootConfig = can_get_boot_config();

	if (bootConfig)
	{
		//read configuration
		can_read_boot_config(bootConfig);
		
		//safety
		bootConfig->module_state = BOOT_NORMAL;
	
		//verify if we have correct configuration
		//write it back if not updated
		if (bootConfig->table_version !=  MODULE_TABLE_VERSION
		|| bootConfig->project_id != MODULE_PROJECT_ID
		|| bootConfig->code_version != MODULE_CODE_VERSION)
		{
			bootConfig->table_version = MODULE_TABLE_VERSION;
			bootConfig->project_id = MODULE_PROJECT_ID;
			bootConfig->code_version = MODULE_CODE_VERSION;

			//CHANGE THE MODULE ID FOR 
			//THE SECOND CONTROLLER...
			bootConfig->module_id = 1;

			can_write_boot_config(bootConfig);

			//THIS WILL UPDATE DEFAULT EEPROM CONFIGURATION FOR ALL MOTORS
			init_default_variables();
		}
	}
	
	//UPDATE CAN ADDRESS
	Can_Addr = bootConfig->module_id;
	
	// init mask
	for(i=0;i<2;i++){
		mask_in[i].mask_priority = 0;
		mask_in[i].mask_type = 0;
		mask_in[i].mask_cmd = 0;
		mask_in[i].mask_dest = 0xFF;
	}
	
	// init filter
	for(i=0;i<6;i++){
		filter_in[i].filter_priority = 0;
		filter_in[i].filter_type = 0;
		filter_in[i].filter_cmd = 0;
		filter_in[i].filter_dest = Can_Addr;
	}

	// init can
	can_init(filter_in,mask_in);
    
	//START INFINITE LOOP
	while(1)
	{
		//READING CAN MESSAGES
		can_transceiver(Can_Addr);
		
		//APPLICATION SPECIFIC CODE HERE
		

	} //while(1)

	return 0;
}


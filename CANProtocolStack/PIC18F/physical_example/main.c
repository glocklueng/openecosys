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

#include "CAN18_Driver.h"
#include "CAN18_Utils.h"
#include "CAN18_Device.h"
#include "CAN18_Shared.h"
#include <string.h>



void init_default_variables(void)
{
	memset(&g_globalCANVariables, 0, sizeof(GlobalCANVariables));
}

void can_proc_message(CAN_MESSAGE *message)
{

}

void main(void)
{
   int i=0;	
   CAN_MASK mask_in[2];
   CAN_FILTER filter_in[6];
   unsigned char canAddr = 0;
   BootConfig *bootConfig = NULL;


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
      filter_in[i].filter_dest = canAddr;
   }

   // init can
   can_init(filter_in,mask_in);


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

			//Set to default address
			bootConfig->module_id = 1;

			//Writing back the boot config for the next version
			can_write_boot_config(bootConfig);

			//set variables to zero
			init_default_variables();
		}
	}
	
	//UPDATE CAN ADDRESS
	canAddr = bootConfig->module_id;


	while( 1 )
	{
		can_transceiver(canAddr);

		//Application specific code here

	} //while(1)
}

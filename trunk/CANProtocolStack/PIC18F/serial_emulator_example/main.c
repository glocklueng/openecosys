#include "NETV8_SerialDriver.h"
#include "NETV8_Device.h"
#include "NETV8_Shared.h"
#include <delays.h>
#include <string.h>
#include "usart.h"

#define TRIS_RX		TRISCbits.TRISC7
#define TRIS_TX		TRISCbits.TRISC6

char buffer = 0;

/***************************************************************************************
ISR IMPLEMENTATIONS
***************************************************************************************/
/* High priority interrupt */
#pragma interrupt highpriority_isr
void highpriority_isr(void)
{
	if(PIR1bits.RC1IF)	//USART1 RX
	{
		//Handle USART Interrupt
		serial_usart_interrupt_handler();
	}
}

/* Low priority interrupt */
#pragma interruptlow lowpriority_isr
void lowpriority_isr(void)
{
    //THIS SHOULD NOT BE CALLED
}

/***************************************************************************************
HIGH PRIORITY INTERRUPT VECTOR
***************************************************************************************/
#pragma code high_vector=0x0008
void high_interrupt (void)
{
    _asm GOTO highpriority_isr _endasm
}

/***************************************************************************************
LOW PRIORITY INTERRUPT VECTOR
***************************************************************************************/
#pragma code low_vector=0x0018
void low_interrupt (void)
{
    _asm GOTO lowpriority_isr _endasm
}


void netv_proc_message(NETV_MESSAGE *message)
{
    //Handle custom messages...
}

void init_default_variables(void)
{
    memset(&g_globalNETVVariables, 0, sizeof(GlobalNETVVariables));
    //g_globalNETVVariables.Var1 = 0;
    //g_globalNETVVariables.Var2 = 5;
}


void main(void)
{

   unsigned char canAddr = 0;
   BootConfig *bootConfig = NULL;
   
   //OSC Interne 16MHz
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 1;    
    //Wait for stabilization
    while(!OSCCONbits.HFIOFS);
    
	//RC6/RC7 as digital:
	ANSELCbits.ANSC7 = 0;
	ANSELCbits.ANSC6 = 0;
	TRIS_RX = 1;
	TRIS_TX = 1;	
	
	//Peripherals
	setup_usart1();	
	buffer = getc_usart1();		//S'assure que le buffer est vide
    
    //reading boot config and device configuration
    //MUST BE DONE BEFORE INITIALIZING NETV MODULE
    bootConfig = netv_get_boot_config();

    if (bootConfig)
    {
            //read configuration
            netv_read_boot_config(bootConfig);

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
                    netv_write_boot_config(bootConfig);

                    //set variables to zero
                    init_default_variables();
            }
    }

    //UPDATE NETV ADDRESS
    canAddr = bootConfig->module_id;
    
   	//Interrupts:
	INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;  

    //Infinite loop
    while (1)
    {
        //Right now will never come out of this function (blocking on serial port)
        netv_transceiver(canAddr);
        g_globalNETVVariables.Var1++;
    }
}



#include "C:\Users\Meka-Intern\Dropbox\Prog\svn_openecosys\PIC18F\CAN18_Serial.h"
#include "C:\Users\Meka-Intern\Dropbox\Prog\svn_openecosys\PIC18F\CAN18_Device.h"
#include "C:\Users\Meka-Intern\Dropbox\Prog\svn_openecosys\PIC18F\CAN18_Shared.h"
#include <delays.h>
#include <string.h>
#include "usart.h"

#define TRIS_RX		TRISCbits.TRISC7
#define TRIS_TX		TRISCbits.TRISC6

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


void can_proc_message(CAN_MESSAGE *message)
{
    //Handle custom messages...
}

void init_default_variables(void)
{
    //memset(&g_globalCANVariables, 0, sizeof(GlobalCANVariables));
    g_globalCANVariables.Var1 = 10;
    g_globalCANVariables.Var2 = 5;
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
	
	//Interrupts:
	INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;  
    
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

    //Infinite loop
    while (1)
    {
        //Right now will never come out of this function (blocking on serial port)
        can_transceiver(canAddr);
    }
}

//Config
#pragma config FOSC = INTIO67, PLLCFG = ON, WDTEN = OFF, LVP = OFF, DEBUG = ON
//#pragma config FCMEN = OFF, BOREN = OFF, PBADEN = OFF, MCLRE = EXTMCLR
//#pragma config STVREN = OFF, XINST = OFF

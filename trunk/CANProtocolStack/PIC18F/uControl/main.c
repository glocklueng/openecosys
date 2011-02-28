//////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                          //
//                                     		[uControl!]                             		//
//                                  			Main                                  		//
//                                       	version 1.0                                     //
//																		                    //
//////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                          //
//                       		  PIC18F44k22, 16MHz xPLL = 16MIPS         					//
//                                         	 JFDuval                                        //
//                                        	30/01/2011         		                        //
//                                                                                          //
//////////////////////////////////////////////////////////////////////////////////////////////

#include "def.h"
#include "..\CAN18_Serial.h"
#include "..\CAN18_Device.h"
#include "..\CAN18_Shared_ucontrol.h"

//ToDo:
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// => 


//////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                          //
//                                    Definitions and variables                             //
//                                                                                          //
//////////////////////////////////////////////////////////////////////////////////////////////

volatile unsigned int refresh = 0;
volatile unsigned int REFRESH_RATE = 30;
volatile char usart_buf_char = 0, buffer = 0;

void init_default_variables(void);
void update_variables(void);


//////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                          //
//                                     	 	Main Function                                 	//
//                                                                                          //
//////////////////////////////////////////////////////////////////////////////////////////////

void main(void)
{	
	//CAN:
   	unsigned char canAddr = 0;
   	BootConfig *bootConfig = NULL;

	//Init
	pre_config();
	config();	

	//Peripherals
	setup_usart1();
	setup_timer1();

	//Test:
	adc_set_channel(10);

	buffer = getc_usart1();		//S'assure que le buffer est vide

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
    
	//Interrupts:
	INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;  

	//Main loop
	while(1)
	{
        //Right now will never come out of this function (blocking on serial port)
        can_transceiver(canAddr);
		update_variables();
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                          //
//                                           Functions                                      //
//                                                                                          //
//////////////////////////////////////////////////////////////////////////////////////////////

//==========================================================================================//
// Function name: 		void pre_config(void)												//
// Short description:	µC config: PLL, etc.												//
// In: 					Nop																	//
// Out:					Nop																	//
//==========================================================================================//

void pre_config(void)
{
	//Oscillator
	
   //OSC Interne 16MHz (62.5ns/inst.)
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 1;
    
    //Wait for stabilization
    while(!OSCCONbits.HFIOFS);
}

//==========================================================================================//
// Function name: 		void config(void)													//
// Short description:	Init peripherals and pins											//
// In: 					Nop																	//
// Out:					Nop																	//
//==========================================================================================//

void config(void)
{
	//Input and output definitions
	define_io();		

}

void can_proc_message(CAN_MESSAGE *message)
{
    //Handle custom messages...
}

void init_default_variables(void)
{
    memset(&g_globalCANVariables, 0, sizeof(GlobalCANVariables));
	g_globalCANVariables.FlashRate = REFRESH_RATE;
}

void update_variables(void)
{
	REFRESH_RATE = g_globalCANVariables.FlashRate;
	g_globalCANVariables.Count = TMR1L;
	g_globalCANVariables.Temp = adc_get_value();		//ToDo: put in an interrupt!! Only for testing 
}


/////////////////////////////////////////////////////////////////////////////
//                                                                         //
//                            Interruptions                                //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

#pragma code highVector=0x08	// on déclare que lors d'une interruption
void atInterrupthigh(void)
{
_asm GOTO Interrupt_High _endasm       // on doit éxecuter le code de la fonction MyHighInterrupt
}
#pragma code // retour à la zone de code


#pragma interrupt Interrupt_High
void Interrupt_High(void)
{
	unsigned char sauv1;
    unsigned char sauv2;

    sauv1 = PRODL; // on sauvegarde le contenu des registres de calcul
    sauv2 = PRODH;       
	
	if(PIR1bits.TMR1IF) // interruption du timer1 
	{
		//LED
		if(refresh < REFRESH_RATE)
		{
			++refresh;
		}
		else
		{
			refresh = 0;
			ALIVE ^= 1;
		}

		PIR1bits.TMR1IF = 0;
	}
	if(PIR1bits.RC1IF)	//USART1 RX
	{
		//Handle USART Interrupt
		serial_usart_interrupt_handler();
		//Echo
		//usart_buf_char = getc_usart1();
		//putc_usart1(usart_buf_char);
	}
				
	PRODL = sauv1;        // on restaure les registres de calcul
    PRODH = sauv2;               
}
#pragma code

/* Low priority interrupt */
#pragma interruptlow lowpriority_isr
void lowpriority_isr(void)
{
    //THIS SHOULD NOT BE CALLED
}


/***************************************************************************************
LOW PRIORITY INTERRUPT VECTOR
***************************************************************************************/
#pragma code low_vector=0x0018
void low_interrupt (void)
{
    _asm GOTO lowpriority_isr _endasm
}

//Config
#pragma config FOSC = INTIO67, PLLCFG = ON, WDTEN = OFF, LVP = OFF, DEBUG = ON

#include "CAN18_Serial.h"
#include "CAN18_Device.h"
#include "CAN18_Shared.h"
#include <usart.h>
#include <delays.h>
#include <string.h>

/***************************************************************************************
ISR IMPLEMENTATIONS
***************************************************************************************/
/* High priority interrupt */
#pragma interrupt highpriority_isr
void highpriority_isr(void)
{

#if 0

    unsigned char value = 0;

    //PIR1 =>Serial RX RCIF
    if(PIR3bits.RC2IF)
    {
        //When RCREG is read it will automatically clear the RCIF flag
        value = RCREG2;

        //Check for overflow errors
        if((RCSTA2bits.OERR) == 1)
        {
                RCSTA2bits.CREN = 0;
                RCSTA2bits.CREN = 1;
        }
        //Check for framing error
        else if(( RCSTA2bits.FERR) == 1)
        {
                value = RCREG2;
        }
        //Data ok, parse it...
        else
        {
                //TODO PARSE SERIAL DATA
        }
    }
#endif

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
    memset(&g_globalCANVariables, 0, sizeof(GlobalCANVariables));
}


void main(void)
{

   unsigned char canAddr = 0;
   BootConfig *bootConfig = NULL;

#if 0
   //OSC Interne 16MHz
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 1;
    
    //Wait for stabilization
    while(!OSCONbits.HFIOFS);
#endif
    
    //Open serial port
    //TODO ADJUST BAUD RATE...
    //TODO USE INTERRUPTS

    OpenUSART2( USART_TX_INT_OFF &
            USART_RX_INT_OFF &
            USART_ASYNCH_MODE &
            USART_EIGHT_BIT &
            USART_CONT_RX &
            USART_BRGH_HIGH,
            25 );


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
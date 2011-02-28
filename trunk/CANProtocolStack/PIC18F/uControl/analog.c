//////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                          //
//                                     		[uControl!]                             		//
//                                         Analog inputs                                	//
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
#include "analog.h"

//Notes: Temp = AN10
//ToDo: Interrupts!

//////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                          //
//                                    Definitions and variables                             //
//                                                                                          //
//////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                          //
//                                           Functions                                      //
//                                                                                          //
//////////////////////////////////////////////////////////////////////////////////////////////

//Setup ADC 10bits
void setup_adc(void)
{
	//Fosc/64 = 1µs conversion time (min.)
	ADCON1bits.PVCFG = 0;	//Vref+ = AVdd
	ADCON1bits.NVCFG = 0;	//Vref- = AVss

	ADCON2bits.ADFM = 1;	//Right justified
	ADCON2bits.ACQT = 4;	//8 Tad (arbitrary)
	ADCON2bits.ADCS	= 6;	//Fosc/64

	ADCON0bits.ADON = 1;	//ADC On
}

void adc_set_channel(unsigned char ch)
{
	ADCON0bits.CHS = ch;
}

unsigned int adc_get_value(void)
{
	ADCON0bits.GO = 1;
	while(ADCON0bits.DONE);
	return ((ADRESH << 8)+ADRESL);
}

unsigned char adc_get_temp(void)
{
//	unsigned int adc_temp = ;					///?!!		//EDIT

//	if(adc_temp > 526)	//>0C	
//		return((adc_temp - 526) >> 3);
//	else
//		return 0;
}

//Return ACS714 current
int adc_get_current(void)
{
}

//Return voltage for specifies channel
unsigned int adc_get_volt(unsigned char ch)
{
}

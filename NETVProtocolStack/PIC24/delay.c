#include "def.h"

//Calibrated for 16MIPS (62.5ns/inst.)

//==========================================================================================//
// Function name: 		void delay_ms(unsigned int t)										//
// Short description:	Wait for t ms														//
// In: 					Nop																	//
// Out:					Nop																	//
//==========================================================================================//
void delay_ms(unsigned int t)
{
	unsigned int time = 0;
	unsigned int repeat = 0;
	
	for(repeat = 0; repeat < t; repeat++)
	{		
	   	for (time = 0; time < 1775; time++) 	//1ms
		{
			Nop();
	   	} 
 	}  	
}

//==========================================================================================//
// Function name: 		void delay_s(unsigned int t)										//
// Short description:	Wait for t s														//
// In: 					Nop																	//
// Out:					Nop																	//
//==========================================================================================//
void delay_s(unsigned int t)
{
	unsigned int repeat = 0;
	
	for(repeat = 0; repeat < t; repeat++)
	{
		delay_ms(1000);		//1s
 	}  	
}

//==========================================================================================//
// Function name: 		void delay_us(unsigned int t)										//
// Short description:	Wait for t µs														//
// In: 					Nop																	//
// Out:					Nop																	//
//==========================================================================================//
void delay_us(unsigned int t)
{
	unsigned int repeat = 0;
	
	for(repeat = 0; repeat < t; repeat++);	//Environ 1.75µs	
}

#include "NETVArduino.h"


typedef union 
{
  struct {
    unsigned int analog0;
	unsigned int analog1;
	unsigned int analog2;
	unsigned int analog3;
	unsigned int analog4;
	unsigned int analog5;
  };
  
  unsigned char NETV_MEM_TABLE[];
} NETVSharedVariables;



NETVSharedVariables variables;



/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */

void setup() { 

  netvArduino.setup(variables.NETV_MEM_TABLE, sizeof(NETVSharedVariables));  
}

void loop() 
{
	netvArduino.transceiver();
	variables.analog0 = analogRead(A0);
	variables.analog1 = analogRead(A1); 
	variables.analog2 = analogRead(A2); 
	variables.analog3 = analogRead(A3); 
	variables.analog4 = analogRead(A4); 
	variables.analog5 = analogRead(A5); 
}

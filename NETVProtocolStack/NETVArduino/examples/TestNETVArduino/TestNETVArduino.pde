#include "NETVArduino.h"


typedef union 
{
  struct {
    float var1;
    float var2;
    float var3;
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
}

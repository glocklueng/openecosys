/*
 
 */
#ifndef NETVArduino_h
#define NETVArduino_h

#include "WProgram.h"

class NETVArduino
{
public:
	
  NETVArduino();
  void setup(byte* table, unsigned int size);	
  
  

private:

  byte* m_table;
  unsigned int m_size;

};

//External instance
extern NETVArduino netvArduino;

#endif

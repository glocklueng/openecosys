#include "NETVArduino.h"

NETVArduino netvArduino;

NETVArduino::NETVArduino()
: m_table(NULL), m_size(0)
{
	
}

void NETVArduino::setup(byte* table, unsigned int size)
{
  if (table && size > 0)
  {
       m_table = table;
       m_size = size;
  }
}

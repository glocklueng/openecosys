#ifndef _BSP_H_
#define _BSP_H_

#include <p30F5015.h>

#define FCY 20000000

//Will use eeprom
#define USE_EEPROM

//Empty struct
typedef struct
{
    unsigned int unused;
} GlobalNETVVariables;

#endif


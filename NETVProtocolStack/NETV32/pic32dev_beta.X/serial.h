#ifndef _SERIAL_H_
#define _SERIAL_H_

#include "GenericTypeDefs.h"
#include <p32xxxx.h>
#include <sys/kmem.h>
#include <plib.h>

//Prototypes
void WriteString(const char *string);
void PutCharacter(char character);

#endif

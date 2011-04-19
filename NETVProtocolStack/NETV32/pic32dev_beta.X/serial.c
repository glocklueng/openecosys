#include "serial.h"
#include <plib.h>

void WriteString(const char *string)
{
    while(*string != '\0')
    {
        while(!UARTTransmitterIsReady(UART1));
        UARTSendDataByte(UART1, *string);
        string++;
        while(!UARTTransmissionHasCompleted(UART1));
    }
}

void PutCharacter(char character)
{
        while(!UARTTransmitterIsReady(UART1));
        UARTSendDataByte(UART1, character);
        while(!UARTTransmissionHasCompleted(UART1));
}

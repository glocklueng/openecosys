#ifndef _BSP_H_
#define _BSP_H_

#include "NETV32_Utils.h"

//Device include & peripheral include
#include "GenericTypeDefs.h"
#include <p32xxxx.h>
#include <sys/kmem.h>
#include <plib.h>


//System clock = 80MHz
#define  SYS_XTAL_FREQ     (60000000L)     // external Xtal, Hz
#define  PB_DIV 1
#define  PRESCALE 256
#define  TOGGLES_PER_SEC 1


//UART BAUD RATE
#define DESIRED_BAUDRATE 1000000L

#define MODULE_TABLE_VERSION 0x02
#define MODULE_PROJECT_ID 0x66
#define MODULE_CODE_VERSION 0x01

#define DEVID_BASE_ADDRESS 0xBF80F220 //See Microchip 61145

#define T1_TICK 10*(SYS_XTAL_FREQ/PB_DIV/PRESCALE/TOGGLES_PER_SEC)


/**
    Define all the variables here to be accessible to the NETV protocol.

    The following data structure will fit with this XML configuration file in the NetworkViewer

    <!DOCTYPE INTROLAB-NETWORKVIEWER>
        <ModuleConfiguration moduleState="-1" deviceID="-1" projectID="1" codeVersion="1" processorID="-1" tableVersion="-1" >
        <ModuleVariable offset="0"  description="Description" type="uint8"  value="" name="Var1" />
        <ModuleVariable offset="1"  description="Description" type="uint16" value="" name="Var2" />
        <ModuleVariable offset="3"  description="Description" type="uint32" value="" name="Var3" />
        <ModuleVariable offset="7"  description="Description" type="sint8"  value="" name="Var4" />
        <ModuleVariable offset="8"  description="Description" type="sint16" value="" name="Var5" />
        <ModuleVariable offset="10" description="Description" type="sint32" value="" name="Var6" />
        <ModuleVariable offset="14" description="Description" type="float"  value="" name="Var7" />
        <ModuleVariable offset="18" description="Description" type="double" value="" name="Var8" />
    </ModuleConfiguration>
*/
typedef struct
{
        uint32 	Var1;
	uint32 	Var2;

} GlobalNETVVariables;


#endif

#include "bsp.h"
#include "NETV32_Common.h"
#include "NETV32_Shared.h"


void netv_proc_message(NETV_MESSAGE *message)
{
    //EMPTY
}




int main()
{


    // Configure the device for maximum performance but do not change the PBDIV
    // Given the options, this function will change the flash wait states, RAM
    // wait state and enable prefetch cache but will not change the PBDIV.
    // The PBDIV value is already set via the pragma FPBDIV option above..
    SYSTEMConfig(SYS_XTAL_FREQ, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);

    //Disable JTAG
    mJTAGPortEnable(DEBUG_JTAGPORT_OFF);


    while(1);

    return 0;
}

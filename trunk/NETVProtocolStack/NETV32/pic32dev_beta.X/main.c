#include "bsp.h"
#include "NETV32_Common.h"
#include "NETV32_Shared.h"
#include "NETV32_SerialDriver.h"


void netv_proc_message(NETV_MESSAGE *message)
{
    //EMPTY
}


void init_default_variables(void)
{
    memset(&g_globalNETVVariables, 0, sizeof(GlobalNETVVariables));
}

void update_variables(void)
{
    //Do something here...


}


int main()
{

    unsigned char canAddr = 0;
    BootConfig *bootConfig = NULL;


    // Configure the device for maximum performance but do not change the PBDIV
    // Given the options, this function will change the flash wait states, RAM
    // wait state and enable prefetch cache but will not change the PBDIV.
    // The PBDIV value is already set via the pragma FPBDIV option above..
    SYSTEMConfig(SYS_XTAL_FREQ, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);

    //Disable JTAG
    mJTAGPortEnable(DEBUG_JTAGPORT_OFF);

    bootConfig = netv_get_boot_config();

    if (bootConfig)
    {
            //read configuration
            netv_read_boot_config(bootConfig);

            //safety
            bootConfig->module_state = BOOT_NORMAL;

            //verify if we have correct configuration
            //write it back if not updated
            if (bootConfig->table_version !=  MODULE_TABLE_VERSION
            || bootConfig->project_id != MODULE_PROJECT_ID
            || bootConfig->code_version != MODULE_CODE_VERSION)
            {
                    bootConfig->table_version = MODULE_TABLE_VERSION;
                    bootConfig->project_id = MODULE_PROJECT_ID;
                    bootConfig->code_version = MODULE_CODE_VERSION;

                    //Set to default address
                    bootConfig->module_id = 1;

                    //Writing back the boot config for the next version
                    netv_write_boot_config(bootConfig);


            }
    }

    //set variables to zero
    init_default_variables();


    //UPDATE NETV ADDRESS
    canAddr = bootConfig->module_id;

    //Open serial port
    setup_usart1();

    while (1)
    {
        //Right now will never come out of this function (blocking on serial port)
        netv_transceiver(canAddr);
        update_variables();
    }



    return 0;
}

//USB software provided by Microchip (USB Device - CDC - BasicDemo)
//Modified by Dominic Letourneau, ing., M.Sc.A. and JFDuval
//IntRoLab - Universite de Sherbrooke and Projet VUE
//http://openecosys.sf.net/

#include "bsp.h"
#include "USB-CDC_Microchip/usb.h"
#include "USB-CDC_Microchip/usb_function_cdc.h"
#include "USB-CDC_Microchip\HardwareProfile.h"
#include "GenericTypeDefs.h"
#include "USB-CDC_Microchip\Compiler.h"
#include "USB-CDC_Microchip\usb_config.h"
#include "USB-CDC_Microchip\usb_device.h"
#include "USB-CDC_Microchip\usb.h"
#include "NETV32_Common.h"
#include "NETV32_Shared.h"
#include "NETV32_SerialDriver.h"

#pragma config FNOSC = PRIPLL
#pragma config FPLLIDIV = DIV_2
#pragma config FPLLMUL = MUL_15         //60MHz
#pragma config FPLLODIV = DIV_1
#pragma config FPBDIV = DIV_1
#pragma config POSCMOD = HS
#pragma config FSOSCEN = OFF
#pragma config IESO = OFF
#pragma config FCKSM = CSDCMD
#pragma config OSCIOFNC = OFF
#pragma config FWDTEN = OFF
#pragma config WDTPS = PS512
#pragma config BWP = OFF
#pragma config CP = OFF
#pragma config ICESEL = ICS_PGx2
#pragma config DEBUG = ON
#pragma config UPLLEN   = ON            // USB PLL Enabled
#pragma config UPLLIDIV = DIV_2         // USB PLL Input Divider


/** P R I V A T E  P R O T O T Y P E S ***************************************/
static void InitializeSystem(void);
void ProcessIO(void);
void USBDeviceTasks(void);
void YourHighPriorityISRCode();
void YourLowPriorityISRCode();

//////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                          //
//                                    Constants and variables                               //
//                                                                                          //
//////////////////////////////////////////////////////////////////////////////////////////////

/** V A R I A B L E S ********************************************************/
char USB_In_Buffer[64];
char USB_Out_Buffer[64];

BOOL stringPrinted;


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
    //Do something useful here...
    g_globalNETVVariables.Var1 += 10000;
    g_globalNETVVariables.Var2 = 1337;

}


//Timer1 ISR: Flash LED
void __ISR(_TIMER_1_VECTOR, ipl2) Timer1Handler(void)
{
    // clear the interrupt flag
    mT1ClearIntFlag();

    mPORTBToggleBits(BIT_12);

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

    //All digital - No Analog inputs
    AD1PCFG = 0xFFFF;
    
    //Disable JTAG
    mJTAGPortEnable(DEBUG_JTAGPORT_OFF);

    //Digital out
    mPORTBClearBits(BIT_12 | BIT_13);
    mPORTBSetPinsDigitalOut( BIT_12 | BIT_13);

    //Configure Timer 1 using internal clock, 1:256 prescale
    OpenTimer1(T1_ON | T1_SOURCE_INT | T1_PS_1_256, T1_TICK);

    // set up the timer interrupt with a priority of 2
    ConfigIntTimer1(T1_INT_ON | T1_INT_PRIOR_2);

    //USB setup
    InitializeSystem();

    #if defined(USB_INTERRUPT)
        USBDeviceAttach();
    #endif

    //NetV:
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

    // Turn on the interrupts
    INTEnableSystemMultiVectoredInt();


    // enable interrupts
    INTEnableInterrupts();

    	//Main loop
	while (1)
	{
		#if defined(USB_POLLING)
        // Check bus status and service USB interrupts.
        USBDeviceTasks();
        // Interrupt or polling method.  If using polling, must call
        // this function periodically.  This function will take care
        // of processing and responding to SETUP transactions
        // (such as during the enumeration process when you first
        // plug in).  USB hosts require that USB devices should accept
        // and process SETUP packets in a timely fashion.  Therefore,
        // when using polling, this function should be called
        // frequently (such as once about every 100 microseconds) at any
        // time that a SETUP packet might reasonably be expected to
        // be sent by the host to your device.  In most cases, the
        // USBDeviceTasks() function does not take very long to
        // execute (~50 instruction cycles) before it returns.
        #endif

        // Application-specific tasks.
        // Application related code may be added here, or in the ProcessIO() function.
        
            ProcessIO();
            update_variables();
            netv_transceiver(canAddr);
	}

    return 0;
}


static void InitializeSystem(void)
{
// The USB specifications require that USB peripheral devices must never source
// current onto the Vbus pin.  Additionally, USB peripherals should not source
//  current on D+ or D- when the host/hub is not actively powering the Vbus line.
// When designing a self powered (as opposed to bus powered) USB peripheral
// device, the firmware should make sure not to turn on the USB module and D+
// or D- pull up resistor unless Vbus is actively powered.  Therefore, the
// firmware needs some means to detect when Vbus is being powered by the host.
// A 5V tolerant I/O pin can be connected to Vbus (through a resistor), and
// can be used to detect when Vbus is high (host actively powering), or low
// (host is shut down or otherwise not supplying power).  The USB firmware
// can then periodically poll this I/O pin to know when it is okay to turn on
// the USB module/D+/D- pull up resistor.  When designing a purely bus powered
// peripheral device, it is not possible to source current on D+ or D- when the
// host is not actively providing power on Vbus. Therefore, implementing this
// bus sense feature is optional.  This firmware can be made to use this bus
// sense feature by making sure "USE_USB_BUS_SENSE_IO" has been defined in the
// HardwareProfile.h file.
    #if defined(USE_USB_BUS_SENSE_IO)
    tris_usb_bus_sense = INPUT_PIN; // See HardwareProfile.h
    #endif

// If the host PC sends a GetStatus (device) request, the firmware must respond
// and let the host know if the USB peripheral device is currently bus powered
// or self powered.  See chapter 9 in the official USB specifications for details
// regarding this request.  If the peripheral device is capable of being both
// self and bus powered, it should not return a hard coded value for this request.
// Instead, firmware should check if it is currently self or bus powered, and
// respond accordingly.  If the hardware has been configured like demonstrated
// on the PICDEM FS USB Demo Board, an I/O pin can be polled to determine the
// currently selected power source.  On the PICDEM FS USB Demo Board, "RA2"
// is used for	this purpose.  If using this feature, make sure "USE_SELF_POWER_SENSE_IO"
// has been defined in HardwareProfile.h, and that an appropriate I/O pin has been mapped
// to it in HardwareProfile.h.
    #if defined(USE_SELF_POWER_SENSE_IO)
    tris_self_power = INPUT_PIN;    // See HardwareProfile.h
    #endif

    USBDeviceInit();    //usb_device.c.  Initializes USB module SFRs and firmware
                        //variables to known states.
}//end InitializeSystem


/********************************************************************
 * Function:        void ProcessIO(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function is a place holder for other user
 *                  routines. It is a mixture of both USB and
 *                  non-USB tasks.
 *
 * Note:            None
 *******************************************************************/
void ProcessIO(void)
{
    // If suspended, do nothing.
    if((USBDeviceState < CONFIGURED_STATE)||(USBSuspendControl==1)) return;

    //NetV Function:
    netv32_usb_task();

    // Service the USB CDC driver
    CDCTxService();

} // End ProcessIO


// ******************************************************************************************************
// ************** USB Callback Functions ****************************************************************
// ******************************************************************************************************
// The USB firmware stack will call the callback functions USBCBxxx() in response to certain USB related
// events.  For example, if the host PC is powering down, it will stop sending out Start of Frame (SOF)
// packets to your device.  In response to this, all USB devices are supposed to decrease their power
// consumption from the USB Vbus to <2.5mA each.  The USB module detects this condition (which according
// to the USB specifications is 3+ms of no bus activity/SOF packets) and then calls the USBCBSuspend()
// function.  You should modify these callback functions to take appropriate actions for each of these
// conditions.  For example, in the USBCBSuspend(), you may wish to add code that will decrease power
// consumption from Vbus to <2.5mA (such as by clock switching, turning off LEDs, putting the
// microcontroller to sleep, etc.).  Then, in the USBCBWakeFromSuspend() function, you may then wish to
// add code that undoes the power saving things done in the USBCBSuspend() function.

// The USBCBSendResume() function is special, in that the USB stack will not automatically call this
// function.  This function is meant to be called from the application firmware instead.  See the
// additional comments near the function.

/******************************************************************************
 * Function:        void USBCBSuspend(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Call back that is invoked when a USB suspend is detected
 *
 * Note:            None
 *****************************************************************************/
void USBCBSuspend(void)
{
    // Insert appropriate code here for the desired
    // application behavior.  If the microcontroller will be put to sleep, a
    // process similar to that shown below may be used:

    /* Example power saving code.

    ConfigureIOPinsForLowPower();
    SaveStateOfAllInterruptEnableBits();
    DisableAllInterruptEnableBits();

    // Should enable at least USBActivityIF as a wake source
    EnableOnlyTheInterruptsWhichWillBeUsedToWakeTheMicro();
    Sleep();

    // Preferrably, this should be done in the USBCBWakeFromSuspend() function instead.
    RestoreStateOfAllPreviouslySavedInterruptEnableBits();

    // Preferrably, this should be done in the USBCBWakeFromSuspend() function instead.
    RestoreIOPinsToNormal();

    // IMPORTANT NOTE: Do not clear the USBActivityIF (ACTVIF) bit here.  This bit is
    // cleared inside the usb_device.c file.  Clearing USBActivityIF here will cause
    //  to not work as intended.
    */

}


/******************************************************************************
 * Function:        void USBCBWakeFromSuspend(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The host may put USB peripheral devices in low power
 *                  suspend mode (by "sending" 3+ms of idle).  Once in suspend
 *                  mode, the host may wake the device back up by sending non-
 *                  idle state signalling.
 *
 *                  This call back is invoked when a wakeup from USB suspend
 *                  is detected.
 *
 * Note:            None
 *****************************************************************************/
void USBCBWakeFromSuspend(void)
{
    // If clock switching or other power savings measures were taken when
    // executing the USBCBSuspend() function, now would be a good time to
    // switch back to normal full power run mode conditions.  The host allows
    // a few milliseconds of wakeup time, after which the device must be
    // fully back to normal, and capable of receiving and processing USB
    // packets.  In order to do this, the USB module must receive proper
    // clocking (IE: 48MHz clock must be available to SIE for full speed USB
    // operation).
}

/********************************************************************
 * Function:        void USBCB_SOF_Handler(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The USB host sends out a SOF packet to full-speed
 *                  devices every 1 ms. This interrupt may be useful
 *                  for isochronous pipes. End designers should
 *                  implement callback routine as necessary.
 *
 * Note:            None
 *******************************************************************/
void USBCB_SOF_Handler(void)
{
    // No need to clear UIRbits.SOFIF to 0 here.
    // Callback caller is already doing that.
}

/*******************************************************************
 * Function:        void USBCBErrorHandler(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The purpose of this callback is mainly for
 *                  debugging during development. Check UEIR to see
 *                  which error causes the interrupt.
 *
 * Note:            None
 *******************************************************************/
void USBCBErrorHandler(void)
{
    // No need to clear UEIR to 0 here.
    // Callback caller is already doing that.

    // Typically, user firmware does not need to do anything special
    // if a USB error occurs.  For example, if the host sends an OUT
    // packet to your device, but the packet gets corrupted (ex:
    // because of a bad connection, or the user unplugs the
    // USB cable during the transmission) this will typically set
    // one or more USB error interrupt flags.  Nothing specific
    // needs to be done however, since the SIE will automatically
    // send a "NAK" packet to the host.  In response to this, the
    // host will normally retry to send the packet again, and no
    // data loss occurs.  The system will typically recover
    // automatically, without the need for application firmware
    // intervention.

    // Nevertheless, this callback function is provided, such as
    // for debugging purposes.
}


/*******************************************************************
 * Function:        void USBCBCheckOtherReq(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        When SETUP packets arrive from the host, some
 *                  firmware must process the request and respond
 *                  appropriately to fulfill the request.  Some of
 *                  the SETUP packets will be for standard
 *                  USB "chapter 9" (as in, fulfilling chapter 9 of
 *                  the official USB specifications) requests, while
 *                  others may be specific to the USB device class
 *                  that is being implemented.  For example, a HID
 *                  class device needs to be able to respond to
 *                  "GET REPORT" type of requests.  This
 *                  is not a standard USB chapter 9 request, and
 *                  therefore not handled by usb_device.c.  Instead
 *                  this request should be handled by class specific
 *                  firmware, such as that contained in usb_function_hid.c.
 *
 * Note:            None
 *******************************************************************/
void USBCBCheckOtherReq(void)
{
    USBCheckCDCRequest();
}//end


/*******************************************************************
 * Function:        void USBCBStdSetDscHandler(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The USBCBStdSetDscHandler() callback function is
 *                  called when a SETUP, bRequest: SET_DESCRIPTOR request
 *                  arrives.  Typically SET_DESCRIPTOR requests are
 *                  not used in most applications, and it is
 *                  optional to support this type of request.
 *
 * Note:            None
 *******************************************************************/
void USBCBStdSetDscHandler(void)
{
    // Must claim session ownership if supporting this request
}//end


/*******************************************************************
 * Function:        void USBCBInitEP(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function is called when the device becomes
 *                  initialized, which occurs after the host sends a
 *                  SET_CONFIGURATION (wValue not = 0) request.  This
 *                  callback function should initialize the endpoints
 *                  for the device's usage according to the current
 *                  configuration.
 *
 * Note:            None
 *******************************************************************/
void USBCBInitEP(void)
{
    CDCInitEP();
}

/********************************************************************
 * Function:        void USBCBSendResume(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The USB specifications allow some types of USB
 *                  peripheral devices to wake up a host PC (such
 *                  as if it is in a low power suspend to RAM state).
 *                  This can be a very useful feature in some
 *                  USB applications, such as an Infrared remote
 *                  control	receiver.  If a user presses the "power"
 *                  button on a remote control, it is nice that the
 *                  IR receiver can detect this signalling, and then
 *                  send a USB "command" to the PC to wake up.
 *
 *                  The USBCBSendResume() "callback" function is used
 *                  to send this special USB signalling which wakes
 *                  up the PC.  This function may be called by
 *                  application firmware to wake up the PC.  This
 *                  function should only be called when:
 *
 *                  1.  The USB driver used on the host PC supports
 *                      the remote wakeup capability.
 *                  2.  The USB configuration descriptor indicates
 *                      the device is remote wakeup capable in the
 *                      bmAttributes field.
 *                  3.  The USB host PC is currently sleeping,
 *                      and has previously sent your device a SET
 *                      FEATURE setup packet which "armed" the
 *                      remote wakeup capability.
 *
 *                  This callback should send a RESUME signal that
 *                  has the period of 1-15ms.
 *
 * Note:            Interrupt vs. Polling
 *                  -Primary clock
 *                  -Secondary clock ***** MAKE NOTES ABOUT THIS *******
 *                   > Can switch to primary first by calling USBCBWakeFromSuspend()

 *                  The modifiable section in this routine should be changed
 *                  to meet the application needs. Current implementation
 *                  temporary blocks other functions from executing for a
 *                  period of 1-13 ms depending on the core frequency.
 *
 *                  According to USB 2.0 specification section 7.1.7.7,
 *                  "The remote wakeup device must hold the resume signaling
 *                  for at lest 1 ms but for no more than 15 ms."
 *                  The idea here is to use a delay counter loop, using a
 *                  common value that would work over a wide range of core
 *                  frequencies.
 *                  That value selected is 1800. See table below:
 *                  ==========================================================
 *                  Core Freq(MHz)      MIP         RESUME Signal Period (ms)
 *                  ==========================================================
 *                      48              12          1.05
 *                       4              1           12.6
 *                  ==========================================================
 *                  * These timing could be incorrect when using code
 *                    optimization or extended instruction mode,
 *                    or when having other interrupts enabled.
 *                    Make sure to verify using the MPLAB SIM's Stopwatch
 *                    and verify the actual signal on an oscilloscope.
 *******************************************************************/
void USBCBSendResume(void)
{
    static WORD delay_count;

    USBResumeControl = 1;       // Start RESUME signaling

    delay_count = 1800U;        // Set RESUME line for 1-13 ms
    do
    {
        delay_count--;
    }while(delay_count);
    USBResumeControl = 0;
}


/*******************************************************************
 * Function:        void USBCBEP0DataReceived(void)
 *
 * PreCondition:    ENABLE_EP0_DATA_RECEIVED_CALLBACK must be
 *                  defined already (in usb_config.h)
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function is called whenever a EP0 data
 *                  packet is received.  This gives the user (and
 *                  thus the various class examples a way to get
 *                  data that is received via the control endpoint.
 *                  This function needs to be used in conjunction
 *                  with the USBCBCheckOtherReq() function since
 *                  the USBCBCheckOtherReq() function is the apps
 *                  method for getting the initial control transfer
 *                  before the data arrives.
 *
 * Note:            None
 *******************************************************************/
#if defined(ENABLE_EP0_DATA_RECEIVED_CALLBACK)
void USBCBEP0DataReceived(void)
{
}
#endif


/*******************************************************************
 * Function:        BOOL USER_USB_CALLBACK_EVENT_HANDLER(
 *                        USB_EVENT event, void *pdata, WORD size)
 *
 * PreCondition:    None
 *
 * Input:           USB_EVENT event - the type of event
 *                  void *pdata - pointer to the event data
 *                  WORD size - size of the event data
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function is called from the USB stack to
 *                  notify a user application that a USB event
 *                  occured.  This callback is in interrupt context
 *                  when the USB_INTERRUPT option is selected.
 *
 * Note:            None
 *******************************************************************/
BOOL USER_USB_CALLBACK_EVENT_HANDLER(USB_EVENT event, void *pdata, WORD size)
{
    switch(event)
    {
        case EVENT_CONFIGURED:
            USBCBInitEP();
            break;
        case EVENT_SET_DESCRIPTOR:
            USBCBStdSetDscHandler();
            break;
        case EVENT_EP0_REQUEST:
            USBCBCheckOtherReq();
            break;
        case EVENT_SOF:
            USBCB_SOF_Handler();
            break;
        case EVENT_SUSPEND:
            USBCBSuspend();
            break;
        case EVENT_RESUME:
            USBCBWakeFromSuspend();
            break;
        case EVENT_BUS_ERROR:
            USBCBErrorHandler();
            break;
        case EVENT_TRANSFER:
            Nop();
            break;
        default:
            break;
    }
    return TRUE;
}

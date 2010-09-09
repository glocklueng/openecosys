/**
     Copyright (C) 2009-2010 IntRoLab
     http://introlab.gel.usherbrooke.ca
     Dominic Letourneau, ing. M.Sc.A.
     Dominic.Letourneau@USherbrooke.ca
 */

#ifndef _PCAN_DEVICE_H_
#define _PCAN_DEVICE_H_

#include "CANDevice.h"


#ifdef WIN32
	#include "windows.h"
        #include "PCANBasic.h"
#else
	#include <libpcan.h>
#endif

/**
     \brief PCANDevice is the front-end to the PCAN driver from PEAK.
     This front-end is useful to send & receive extended CAN messages
     from the bus. Extended IDs are used to fill LABORIUS_MESSAGE
     data structures, according to the LABORIUS protocol. Filters and
     masks can also be used to identify special user-defined messages.
     This front-end works with any PCAN device supported by the PEAK driver
     (USB, Parallel,PCI). This front-end will read all the messages
     coming on the bus. The filters are only useful for tagging the messages
     with the filter-id. Users don't need to set the masks and filters for
     the front-end to work properly.
     
     IMPORTANT : PEAK CAN Linux driver must be installed before compiling this
                 front-end from :  http://www.peak-system.com/linux/

     \authors $Author: dominic $
     \version $Revision: 1.1 $
     \date $Date: 2007-01-11 19:21:20 $
*/
class PCANDevice : public CANDevice
{
     public:

      /** Constructor with a device name
           This contructor will call connect automatically with the
           device specified.
           \param device The can device [ex : /dev/pcan32]
      */
      PCANDevice(const char* device = "/dev/pcan32");


      /** Destructor */
      virtual ~PCANDevice();



      /** send a LABORIUS_MESSAGE
           \param message The message to send
           \return int The status after the message has been sent
      */
      virtual CANDevice::State sendMessage(LABORIUS_MESSAGE &message);

      /** receive a LABORIUS_MESSAGE
           \param message The message to receive (will be filled)
           \return int The status after the message has been received
      */
      virtual CANDevice::State recvMessage(LABORIUS_MESSAGE &message);

    

        /** initialize the PCAN device
             \param device the device path
        */
        virtual CANDevice::State initialize(const char* device);

        /** Verify if a message is ready to receive
             \return bool true if a message is ready to be received
        */
        virtual bool newMessageReady();

	
	private:

	/** Check the status of the PCAN device
		 \param debug true=print status message
	*/
	DWORD checkStatus(bool debug=false);


    /// PCAN driver handler
    HANDLE m_handle;
};



#endif

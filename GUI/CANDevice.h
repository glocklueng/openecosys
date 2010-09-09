/**
     Copyright (C) 2009-2010 IntRoLab
     http://introlab.gel.usherbrooke.ca
     Dominic Letourneau, ing. M.Sc.A.
     Dominic.Letourneau@USherbrooke.ca
 */

#ifndef _CAN_DEVICE_H_
#define _CAN_DEVICE_H_

#include <list>
#include <string>
#include <vector>
#include <iostream>
#include <string.h>

#include <QString>
#include <QStringList>
#include <QMap>
#include <QDir>
#include <QLibrary>

//TODO Remove this
using namespace std;

/**
     \brief LABORIUS_MESSAGE
     
     This data structure holds CAN message informations
     to be written to the CAN bus or read from the
     CAN bus. The 29 bit extended frames are separated into
     LABORIUS home made protocol and fields.
     
     \author Dominic Letourneau
     \version Revision: 1.1
     \date 2007-01-11 19:21:20
*/
typedef struct _message
{
     /** Default Constructor */
     _message()
     {
          msg_priority = 0;
          msg_type = 0;
          msg_cmd = 0;
          msg_boot = 0;
          msg_dest = 0;
          msg_data_length =0;
          msg_remote = 0;
          memset(msg_data,0,8);
          msg_filter_hit=-1;
          msg_dwTime = 0;               
     }
     /** Copy Constructor
          \param message The message to copy
     */
     _message(const _message &message) {    
          msg_priority=message.msg_priority;
          msg_type=message.msg_type;
          msg_cmd=message.msg_cmd;
          msg_boot=message.msg_boot;
          msg_dest=message.msg_dest;
          msg_data_length=message.msg_data_length;
          memcpy(msg_data,message.msg_data,8);
          msg_remote=message.msg_remote;
          msg_filter_hit=message.msg_filter_hit;
          msg_dwTime = message.msg_dwTime;
     }
     /// Message priority 0=high to 7=low
     unsigned char msg_priority; 
     /// Message type (8 bit mask)
     unsigned char msg_type;
     ///Message command 0 to 255   
     unsigned char msg_cmd;
     ///Message boot bits used to program PIC devices 0 to 3 (boot bits)
     unsigned char msg_boot;
     ///Message destination 0 to 255, 255 = broadcast
     unsigned char msg_dest;
     ///Message data length 0 to 8 bytes
     char msg_data_length;
     ///Message data maximum 8 bytes
     unsigned char msg_data[8];
     ///Message remote request bit (RTR)
     unsigned char msg_remote;
     ///Filter hit (used by the PCANDevice driver)
     int msg_filter_hit;
     ///Time stamp (ms)
     unsigned int msg_dwTime;
     
     
} LABORIUS_MESSAGE;

/**

     \brief LABORIUS_FILTER
     
     A combination of mask and filter is done to determine if
     we have a filter hit. This can be used to detect particular
     types of messages. We have a hit if :
     (LABORIUS_FILTER & LABORIUS_MASK) ^ (LABORIUS_MESSAGE & LABORIUS_MASK)  == 0
     This means that we do not have any difference between
     the masked message and the masked filter (binary operators).
     \author Dominic Letourneau
     \version Revision: 1.1
     \date Date: 2007-01-11 19:21:20
*/
typedef struct _filter
{
              
     /** Default Constructor */
     _filter()
     {
          filter_priority = 0;
          filter_type = 0;
          filter_cmd = 0;
          filter_dest = 0;
     }
     /** Constructor
          \param priority filter priority
          \param type filter type
          \param cmd filter command
          \param dest filter destination
     */
     _filter(unsigned char priority,unsigned char type,unsigned char cmd,unsigned char dest)
     {
          filter_priority=priority;
          filter_type=type;
          filter_cmd=cmd;
          filter_dest=dest;
     }
     /** Copy constructor
          \param filter The filter to copy
     */
     _filter(const _filter &filter)
     {
          filter_priority=filter.filter_priority;
          filter_type=filter.filter_type;
          filter_cmd=filter.filter_cmd;
          filter_dest=filter.filter_dest;
     }
     ///message priority filter
     unsigned char filter_priority;
     ///message type filter
     unsigned char filter_type;
     ///message command filter
     unsigned char filter_cmd;
     ///message destination filter
     unsigned char filter_dest;
} LABORIUS_FILTER;

/**
     \brief LABORIUS_MASK
     
     A combination of mask and filter is done to determine if
     we have a filter hit. This can be used to detect particular
     types of messages. We have a hit if :
     (LABORIUS_FILTER & LABORIUS_MASK) ^ (LABORIUS_MESSAGE & LABORIUS_MASK)  == 0
     This means that we do not have any difference between
     the masked message and the masked filter (binary operators).
     \author Dominic Letourneau
     \version $Revision: 1.1 $
     \date $Date: 2007-01-11 19:21:20 $
*/
typedef struct _mask
{
     /** Default Constructor */
     _mask()
     {
          mask_priority = 0;
          mask_type = 0;
          mask_cmd = 0;
          mask_dest = 0;
     }
     /** Constructor
          \param priority filter priority
          \param type filter type
          \param cmd filter command
          \param dest filter destination
     */
     _mask(unsigned char priority,unsigned char type,unsigned char cmd,unsigned char dest)
     {
          mask_priority=priority;
          mask_type=type;
          mask_cmd=cmd;
          mask_dest=dest;
     }
     /** Copy Constructor
          \param mask the mask to copy
     */
     _mask(const _mask &mask)
     {
          mask_priority=mask.mask_priority;
          mask_type=mask.mask_type;
          mask_cmd=mask.mask_cmd;
          mask_dest=mask.mask_dest;
     }
     ///mask priority
     unsigned char mask_priority;
     ///mask type
     unsigned char mask_type;
     ///mask command
     unsigned char mask_cmd;
     ///mask destination
     unsigned char mask_dest;
} LABORIUS_MASK;


///DATA TABLES MEM TYPES
enum
{
     CAN_MEM_TYPE_RAM=0, CAN_MEM_TYPE_EEPROM=1
};


/**


*/
class CANDevice 
{

    

    public:


        /**
         * Basic factory that can create a device. This is a virtual class that needs to be
         * derived from.
         */
        class BaseDeviceFactory
        {
                public:


		    /**
			    Constructor
			    \param default_args The default arguments of the driver
			    \param documentation The online documentation of the driver
		    */
		    BaseDeviceFactory(const QString &default_args = QString(), const QString &documentation = QString())
			    :	m_defaultArgs(default_args), m_documentation(documentation)
		    {

		    }

            /**
             * This virtual function must be implemented and is used to create
             * A new device related to the NetworkView.
             * \param args Driver arguments
             */
            virtual CANDevice* create(const char* args) = 0;

            /**
                \return The default arguments of the driver
            */
		    QString getDefaultArgs()
		    {
			    return m_defaultArgs;
		    }	

            /**
                \return The documentation of the driver
            */
		    QString getDocumentation()
		    {
			    return m_documentation;
		    }

		    ///The default arguments of the driver
		    QString m_defaultArgs;

		    ///The documentation of the driver (online)
		    QString m_documentation;
        };

        /**
         * Template class for quick and easy device factory creation.
         * A factory can be created by using the template like this :
         * DeviceFactory::BaseDeviceFactory *myFactory = new BaseDeviceFactory::DeviceFactory<class name>();
         */
        template <class T>
        class DeviceFactory : public BaseDeviceFactory
        {
        public:
		
		    /**
			    Constructor
			    \param default_args The default arguments of the driver
			    \param documentation The online documentation of the driver
		    */
		    DeviceFactory(const QString &default_args = QString(), const QString &documentation = QString())
			    : BaseDeviceFactory(default_args, documentation)
		    {

		    }

            /**
             * Overloaded from BaseDeviceFactory. This will create a new device
             * \param args Driver arguments
             */
            virtual CANDevice* create(const char* args)
            {
                    return new T(args);
            }

		

        };





          ///Device states
          typedef enum {CANDEVICE_OK,
                        CANDEVICE_FAIL,
                        CANDEVICE_OVERFLOW,
                        CANDEVICE_UNDERFLOW,
                        CANDEVICE_BUS,
                        CANDEVICE_NOT_INITIALIZED} State;


          //Will initialize with defined arguments
          virtual State initialize(const char* args) = 0;


          /** send a LABORIUS_MESSAGE
               \param message The message to send
               \return State The status after the message has been sent
          */
          virtual State sendMessage(LABORIUS_MESSAGE &message) = 0;

          /** receive a LABORIUS_MESSAGE
               \param message The message to receive (will be filled)
               \return int The status after the message has been received
          */
          virtual State recvMessage(LABORIUS_MESSAGE &message) = 0;

          /** Verify if a message is ready to receive
               \return bool true if a message is ready to be received
          */
          virtual bool newMessageReady() = 0;

          /** Add a filter to verify incoming messages
               \param filter the filter to be added               
          */
          void addFilter(LABORIUS_FILTER &filter);

          /** Add a mask to very incoming messages
               \param mask the mask to be added
          */
          void addMask(LABORIUS_MASK &mask);

          /** Get the masks used for filtering
              \return vector<LABORIUS_MASK> All the masks used for filtering
          */
          vector<LABORIUS_MASK> & getMasks() {return m_masks;}

          /** Get the filters used for filtering (hits)
               \return vector<LABORIUS_FILTER> All the filters used for filtering
          */
          vector<LABORIUS_FILTER> & getFilters() {return m_filters;}

          /** Print a LABORIUS_MESSAGE
               \param message The message to print
               \param out The stream to use for printing (default = stdout)
          */                             
          static void printMessage(const LABORIUS_MESSAGE &message, ostream &out = cout);

          /** Clear all the masks */
          void clearMasks(){m_masks.resize(0);}

          /** Clear all the filters */
          void clearFilters(){m_filters.resize(0);}

          /**
                This static function will be called to register a device when loading a dynamic library.
                Device factories need to use an unique name and a factory pointer.
                \param name The factory name for the device driver
                \param factory the factory used for device creation
                \return bool TRUE if the factory was registered successfully
          */
          static bool registerDeviceFactory(const QString& name, BaseDeviceFactory* factory);

          /**
                Create a device with its name and arguments
                \param name the name of the factory
                \param args the arguments for the device cration in a string
          */
          static CANDevice* createDevice(const QString& name, const char* args);

          /**
                Will return all the factory names in a list
                \return QStringList The factory name list
          */
          static QStringList deviceList();

          /**
           * Will scan plugins (dll, so, dyld files) from a base directory. Will also
           * traverse all sub-directory.
           * \param basePath The path where to start the scan.
           */
          static void scanDrivers(const QString &basePath);


          /**
                \param name The factory name
                \return BaseDeviceFactory* the factory, NULL if not available
          */
          static BaseDeviceFactory* getFactoryNamed(const QString &name);

     protected:

          static QMap<QString,BaseDeviceFactory*>& getFactoryMap();

          /**
           * Internal function to traverse directory for library scans.
           * \param directory The current QDir
           * \param level the recursion level, 0 = basePath
           */
          static void recursiveScan(QDir directory, int level = 0);

          /// Software masks
          vector<LABORIUS_MASK> m_masks;

          /// Software filters
          vector<LABORIUS_FILTER> m_filters;

          /** Scan for filter hits
               \param message the message to verify
          */
          void applyFilters(LABORIUS_MESSAGE &message);

};


#endif

/**
     Copyright (C) 2009-2010 IntRoLab
     http://introlab.gel.usherbrooke.ca
     Dominic Letourneau, ing. M.Sc.A.
     Dominic.Letourneau@USherbrooke.ca

     This file is part of OpenECoSys/NetworkViewer.
     OpenECoSys/NetworkViewer is free software: you can redistribute it and/or modify
     it under the terms of the GNU General Public License as published by the Free Software
     Foundation, either version 3 of the License, or (at your option) any later version.
     OpenECoSys/NetworkViewer is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
     or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
     You should have received a copy of the GNU General Public License along with
     OpenECoSys/NetworkViewer. If not, see http://www.gnu.org/licenses/.

 */
#ifndef _NETV_DEVICE_H_
#define _NETV_DEVICE_H_

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
#include <QIODevice>

//TODO Remove this
using namespace std;

/**
     \brief NETV_MESSAGE
     
     This data structure holds NETV message informations
     to be written to the NETV bus or read from the
     NETV bus. The 29 bit extended frames are separated into
     LABORIUS home made protocol and fields.
     
     \author Dominic Letourneau
     \version Revision: 1.1
     \date 2007-01-11 19:21:20
*/
typedef class _message
{

public:

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
          msg_timestamp = 0;
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
          msg_timestamp = message.msg_timestamp;
     }

     _message& operator=(const _message &message)
     {
         msg_priority=message.msg_priority;
         msg_type=message.msg_type;
         msg_cmd=message.msg_cmd;
         msg_boot=message.msg_boot;
         msg_dest=message.msg_dest;
         msg_data_length=message.msg_data_length;
         memcpy(msg_data,message.msg_data,8);
         msg_remote=message.msg_remote;
         msg_filter_hit=message.msg_filter_hit;
         msg_timestamp = message.msg_timestamp;
         return *this;
     }

     bool operator==(const _message &message)
     {
         if (msg_priority != message.msg_priority) return false;
         if (msg_type != message.msg_type) return false;
         if (msg_cmd != message.msg_cmd) return false;
         if (msg_boot != message.msg_boot) return false;
         if (msg_dest != message.msg_dest) return false;
         if (msg_data_length != message.msg_data_length) return false;
         if (msg_timestamp != msg_timestamp) return false;
         if (message.msg_data_length <= 8)
         {
             for (unsigned int i = 0; i < message.msg_data_length; i++)
             {
                 if (msg_data[i] != message.msg_data[i]) return false;
             }
         }
         else
         {
             return false;
         }

         if (msg_remote != message.msg_remote) return false;
         return true;
     }


     bool serialize(QIODevice &dev) const
     {
         if (dev.isWritable())
         {
             dev.write(QByteArray("NETV_MESSAGE;"));
             dev.write(QByteArray((char*)&msg_priority,1));
             dev.write(QByteArray((char*)&msg_type,1));
             dev.write(QByteArray((char*)&msg_cmd,1));
             dev.write(QByteArray((char*)&msg_boot,1));
             dev.write(QByteArray((char*)&msg_dest,1));
             dev.write(QByteArray((char*)&msg_remote,1));
             dev.write(QByteArray((char*)&msg_timestamp, sizeof(qint64)));

             //Size
             dev.write(QByteArray((char*)&msg_data_length,1));

             //Wrote 28 bytes before data
             dev.write(QByteArray((char*)&msg_data[0],(unsigned int) msg_data_length));

             return true;
         }
         else
         {
            return false;
         }
     }

     bool unserialize(QIODevice &dev)
     {
         if (dev.isReadable() && dev.bytesAvailable() >= 28)
         {
            QByteArray peekData = dev.peek(28);
            QByteArray name = peekData.mid(0,13);
            QByteArray header = peekData.mid(13,15);

            if (name != QByteArray("NETV_MESSAGE;"))
            {
                return false;
            }
            else
            {
                //We have the right object...
                msg_priority = header[0];
                msg_type = header[1];
                msg_cmd = header[2];
                msg_boot = header[3];
                msg_dest = header[4];
                msg_remote = header[5];


                memcpy((char*) &msg_timestamp,&header.data()[6],sizeof(qint64));

                msg_data_length = header[14];

                //Read data
                if (dev.bytesAvailable() >= (28 + msg_data_length) && msg_data_length <= 8)
                {
                    //read header
                    dev.read(28);

                    //read data
                    dev.read((char*)&msg_data[0],(unsigned int)msg_data_length);
                }
                else
                {
                    return false;
                }


                return true;
            }
         }


         return false;
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
     qint64 msg_timestamp;
} NETV_MESSAGE;

/**

     \brief NETV_FILTER
     
     A combination of mask and filter is done to determine if
     we have a filter hit. This can be used to detect particular
     types of messages. We have a hit if :
     (NETV_FILTER & NETV_MASK) ^ (NETV_MESSAGE & NETV_MASK)  == 0
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
} NETV_FILTER;

/**
     \brief NETV_MASK
     
     A combination of mask and filter is done to determine if
     we have a filter hit. This can be used to detect particular
     types of messages. We have a hit if :
     (NETV_FILTER & NETV_MASK) ^ (NETV_MESSAGE & NETV_MASK)  == 0
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
} NETV_MASK;





/**


*/
class NETVDevice
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
            virtual NETVDevice* create(const char* args) = 0;

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


                    //Configure
                    virtual QString configure();
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
            virtual NETVDevice* create(const char* args)
            {
                    return new T(args);
            }

            /**
                Configure can be specialized if you want to display a specialized GUI for your driver
            */
            virtual QString configure()
            {
                return BaseDeviceFactory::configure();
            }

		

        };

          ///Device states
          typedef enum {NETVDEVICE_OK,
                        NETVDEVICE_FAIL,
                        NETVDEVICE_OVERFLOW,
                        NETVDEVICE_UNDERFLOW,
                        NETVDEVICE_BUS,
                        NETVDEVICE_NOT_INITIALIZED} State;

          NETVDevice()
	  {
                qDebug("NETVDevice Constructor");
	  }

          virtual ~NETVDevice()
          {
                qDebug("NETVDevice Destructor");
          }
	 

          //Will initialize with defined arguments
          virtual State initialize(const char* args) = 0;


          ///Return device name
          QString getName();

          ///Return device args
          QString getArgs();

          /** send a NETV_MESSAGE
               \param message The message to send
               \return State The status after the message has been sent
          */
          virtual State sendMessage(NETV_MESSAGE &message) = 0;

          /** receive a NETV_MESSAGE
               \param message The message to receive (will be filled)
               \return int The status after the message has been received
          */
          virtual State recvMessage(NETV_MESSAGE &message) = 0;

          /** Verify if a message is ready to receive
               \return bool true if a message is ready to be received
          */
          virtual bool newMessageReady() = 0;

          /** Add a filter to verify incoming messages
               \param filter the filter to be added               
          */
          void addFilter(NETV_FILTER &filter);

          /** Add a mask to very incoming messages
               \param mask the mask to be added
          */
          void addMask(NETV_MASK &mask);

          /** Get the masks used for filtering
              \return vector<NETV_MASK> All the masks used for filtering
          */
          vector<NETV_MASK> & getMasks() {return m_masks;}

          /** Get the filters used for filtering (hits)
               \return vector<NETV_FILTER> All the filters used for filtering
          */
          vector<NETV_FILTER> & getFilters() {return m_filters;}

          /** Print a NETV_MESSAGE
               \param message The message to print
               \param out The stream to use for printing (default = stdout)
          */                             
          static void printMessage(const NETV_MESSAGE &message, ostream &out = cout);

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
          static NETVDevice* createDevice(const QString& name, const char* args);

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

          /** Scan for filter hits
               \param message the message to verify
          */
          void applyFilters(NETV_MESSAGE &message);


          static QMap<QString,BaseDeviceFactory*>& getFactoryMap();

          /**
           * Internal function to traverse directory for library scans.
           * \param directory The current QDir
           * \param level the recursion level, 0 = basePath
           */
          static void recursiveScan(QDir directory, int level = 0);

          /// Software masks
          vector<NETV_MASK> m_masks;

          /// Software filters
          vector<NETV_FILTER> m_filters;

          /// The device name
          QString m_name;

          /// The device arguments
          QString m_args;

};


#endif

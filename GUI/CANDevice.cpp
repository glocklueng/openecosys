/**
     Copyright (C) 2009-2010 IntRoLab
     http://introlab.gel.usherbrooke.ca
     Dominic Letourneau, ing. M.Sc.A.
     Dominic.Letourneau@USherbrooke.ca
 */


#include "CANDevice.h"
#include <QDebug>

using namespace std;

void CANDevice::printMessage(const LABORIUS_MESSAGE &message, ostream &out) {

     out<<"msg_priority    : "<<(unsigned int)message.msg_priority<<endl;
     out<<"msg_type        : "<<(unsigned int)message.msg_type<<endl;
     out<<"msg_cmd         : "<<(unsigned int)message.msg_cmd<<endl;
     out<<"msg_dest        : "<<(unsigned int)message.msg_dest<<endl;
     out<<"msg_boot        : "<<(unsigned int)message.msg_boot<<endl;
     out<<"msg_data_length : "<<(unsigned int)message.msg_data_length<<endl;
     out<<"msg_remote      : "<<(unsigned int)message.msg_remote<<endl;
     out<<"msg_data        : ";
     for (int i = 0; i < min(8,(int)message.msg_data_length); i++) {
          out<<(unsigned int)message.msg_data[i]<<" ";
     }
     out<<endl;
     out<<"msg_filter_hit  : "<<message.msg_filter_hit<<endl;
     out<<"msg_dwTime      : "<<message.msg_dwTime<<endl;
     out<<endl<<endl;

}

void CANDevice::addFilter(LABORIUS_FILTER &filter)
{
     m_filters.push_back(filter);
}

void CANDevice::addMask(LABORIUS_MASK &mask)
{

     m_masks.push_back(mask);
}

void CANDevice::applyFilters(LABORIUS_MESSAGE &message)
{

     //default : message did not match any filter
     message.msg_filter_hit = -1;

     for (unsigned int i = 0; i < m_masks.size(); i++) {
          for (unsigned int j = 0; j < m_filters.size(); j++) {
      
               int msg_value = (((int) message.msg_priority << 24)  & 0x03000000);
               msg_value |= (((int) message.msg_type << 16) & 0x00FF0000);
               msg_value |= (((int) message.msg_cmd << 8) & 0x0000FF00);
               msg_value |= (((int) message.msg_dest) & 0x000000FF);

               int mask_value =  (((int) m_masks[i].mask_priority << 24)  & 0x03000000);
               mask_value |= (((int) m_masks[i].mask_type << 16) & 0x00FF0000);
               mask_value |= (((int) m_masks[i].mask_cmd << 8) & 0x0000FF00);
               mask_value |= (((int) m_masks[i].mask_dest) & 0x000000FF);

               int filter_value =  (((int) m_filters[i].filter_priority << 24)  & 0x03000000);
               filter_value |= (((int) m_filters[i].filter_type << 16) & 0x00FF0000);
               filter_value |= (((int) m_filters[i].filter_cmd << 8) & 0x0000FF00);
               filter_value |= (((int) m_filters[i].filter_dest) & 0x000000FF);
               
               //look at difference in bits
               int xor_filter = (msg_value & mask_value) ^ (filter_value & mask_value); 
      
               if (!xor_filter) {
                    message.msg_filter_hit = j;
               }      
          }
     }
}




QMap<QString,CANDevice::BaseDeviceFactory*>& CANDevice::getFactoryMap()
 {
    static QMap<QString,BaseDeviceFactory*> myMap;
    return myMap;
 }


CANDevice::BaseDeviceFactory* CANDevice::getFactoryNamed(const QString &name)
{
    if(CANDevice::getFactoryMap().contains(name))
    {
        return CANDevice::getFactoryMap()[name];
    }
    else
    {
        return NULL;
    }
}


bool CANDevice::registerDeviceFactory(const QString& name, CANDevice::BaseDeviceFactory* factory)
{
    if (CANDevice::getFactoryMap().find(name) == CANDevice::getFactoryMap().end())
    {
        CANDevice::getFactoryMap().insert(name,factory);
        qDebug() << "Registering device :" << name;
        return true;
    }
    else
    {
        qDebug() << "Device already exist, unable to register device : " << name;
        return false;
    }
}

 CANDevice* CANDevice::createDevice(const QString& name, const char* args)
 {
     CANDevice *dev = NULL;

     if (CANDevice::getFactoryMap().find(name) != CANDevice::getFactoryMap().end())
     {
         dev = CANDevice::getFactoryMap()[name]->create(args);
     }

     return dev;
 }

QStringList CANDevice::deviceList()
 {
    QStringList output;
    for(QMap<QString,BaseDeviceFactory*>::iterator iter = CANDevice::getFactoryMap().begin(); iter != CANDevice::getFactoryMap().end(); iter++)
    {
        output.push_back(iter.key());
    }
    return output;
 }

void CANDevice::scanDrivers(const QString &basePath)
{
    qDebug("CANDevice::scanDrivers()");

    //Plugins directory
    QDir dir(basePath);

    if (!dir.exists())
    {
            qWarning("CANDevice::scanDrivers() : drivers directory not found");
            qWarning() << "CANDevice::scanDrivers() : Current path : " << dir.absolutePath();
    }
    else
    {
            qDebug() << "CANDevice::scanDrivers() : Scanning : " << dir.absolutePath();
            recursiveScan(dir);
    }


}


void CANDevice::recursiveScan(QDir directory, int level)
{
        qDebug() << "CANDevice::recursiveScan :  Path :" << directory.path();

        if (level < 10 && directory.exists())
        {

                QFileInfoList myInfoList = directory.entryInfoList();

                for (int i = 0; i < myInfoList.size(); i++)
                {

                        if (!myInfoList[i].fileName().startsWith("."))
                        {
                                if (myInfoList[i].isDir())
                                {
                                        recursiveScan(QDir(directory.path() + QDir::separator() + myInfoList[i].fileName()), level + 1);
                                }
                                else
                                {
                                        //standard file
                                        //TODO Unix dlls
#ifdef WIN32
                                        if (myInfoList[i].fileName().contains(".dll") && !myInfoList[i].fileName().contains(".dll.a")) {
#else
					if (myInfoList[i].fileName().contains(".so") || myInfoList[i].fileName().contains(".dylib")) {
#endif
                                                qDebug() << "CANDevice::recursiveScan : Loading library : " << directory.path() + QDir::separator() + myInfoList[i].fileName();

                                                QLibrary *library = new QLibrary(directory.path() + QDir::separator() + myInfoList[i].fileName());

                                                if (library->load())
                                                {
                                                        qDebug() << "BaseCANDevice::recursiveScan : Loaded : " << directory.path() + QDir::separator() + myInfoList[i].fileName();


                                                        //TODO : here would be the place to call an init function for the library...
                                                        //TODO : do something with the library pointer...
                                                }
                                                else
                                                {
                                                        qDebug() << "CANDevice::recursiveScan : Error : " << library->errorString();
                                                }
                                        }
                                }
                        }//starts with "."
                }//for infoList
        }
        else
        {
                qWarning("BasePlugin::recursiveScan : error level : %i",level);
        }
}

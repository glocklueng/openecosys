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


#include "NETVDevice.h"
#include <QDebug>
#include <QMessageBox>

using namespace std;


QString NETVDevice::BaseDeviceFactory::configure()
{
    QMessageBox msgBox;
    msgBox.setText("No configuration dialog available.");
    msgBox.exec();
    return QString();
}

void NETVDevice::printMessage(const NETV_MESSAGE &message, ostream &out) {

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
    out<<"msg_timestamp      : "<<message.msg_timestamp<<endl;
    out<<endl<<endl;

}

void NETVDevice::addFilter(NETV_FILTER &filter)
{
    m_filters.push_back(filter);
}

void NETVDevice::addMask(NETV_MASK &mask)
{

    m_masks.push_back(mask);
}

void NETVDevice::applyFilters(NETV_MESSAGE &message)
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



QMap<QString,NETVDevice::BaseDeviceFactory*>& NETVDevice::getFactoryMap()
{
    static QMap<QString,BaseDeviceFactory*> myMap;
    return myMap;
}


NETVDevice::BaseDeviceFactory* NETVDevice::getFactoryNamed(const QString &name)
{
    if(NETVDevice::getFactoryMap().contains(name))
    {
        return NETVDevice::getFactoryMap()[name];
    }
    else
    {
        return NULL;
    }
}


bool NETVDevice::registerDeviceFactory(const QString& name, NETVDevice::BaseDeviceFactory* factory)
{
    if (NETVDevice::getFactoryMap().find(name) == NETVDevice::getFactoryMap().end())
    {
        NETVDevice::getFactoryMap().insert(name,factory);
        qDebug() << "Registering device :" << name;
        return true;
    }
    else
    {
        qDebug() << "Device already exist, unable to register device : " << name;
        return false;
    }
}

NETVDevice* NETVDevice::createDevice(const QString& name, const char* args)
{
    NETVDevice *dev = NULL;

    if (NETVDevice::getFactoryMap().find(name) != NETVDevice::getFactoryMap().end())
    {
        dev = NETVDevice::getFactoryMap()[name]->create(args);

        //Remember name & args
        dev->m_name = name;
        dev->m_args = QString(args);
    }

    return dev;
}

QStringList NETVDevice::deviceList()
{
    QStringList output;
    for(QMap<QString,BaseDeviceFactory*>::iterator iter = NETVDevice::getFactoryMap().begin(); iter != NETVDevice::getFactoryMap().end(); iter++)
    {
        output.push_back(iter.key());
    }
    return output;
}

void NETVDevice::scanDrivers(const QString &basePath)
{
    qDebug("NETVDevice::scanDrivers()");

    //Plugins directory
    QDir dir(basePath);

    if (!dir.exists())
    {
        qWarning("NETVDevice::scanDrivers() : drivers directory not found");
        qWarning() << "NETVDevice::scanDrivers() : Current path : " << dir.absolutePath();
    }
    else
    {
        qDebug() << "NETVDevice::scanDrivers() : Scanning : " << dir.absolutePath();
        recursiveScan(dir);
    }


}


void NETVDevice::recursiveScan(QDir directory, int level)
{
    qDebug() << "NETVDevice::recursiveScan :  Path :" << directory.path();

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
                        qDebug() << "NETVDevice::recursiveScan : Loading library : " << directory.path() + QDir::separator() + myInfoList[i].fileName();

                        QLibrary *library = new QLibrary(directory.path() + QDir::separator() + myInfoList[i].fileName());

                        if (library->load())
                        {
                            qDebug() << "BaseCANDevice::recursiveScan : Loaded : " << directory.path() + QDir::separator() + myInfoList[i].fileName();


                            //TODO : here would be the place to call an init function for the library...
                            //TODO : do something with the library pointer...
                        }
                        else
                        {
                            qDebug() << "NETVDevice::recursiveScan : Error : " << library->errorString();
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



QString NETVDevice::getName()
{
    return m_name;
}

QString NETVDevice::getArgs()
{
    return m_args;
}

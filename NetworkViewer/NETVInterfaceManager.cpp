/**
     Copyright (C) 2009-2011 IntRoLab
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


#include "NETVInterfaceManager.h"
#include "NETVMessageEvent.h"

NETVInterfaceManager::NETVInterfaceManager(NETVDevice *device, const char* args, QObject *parent)
    :    QObject(parent), m_handler(NULL), m_scheduler(NULL)
{

    if (device)
    {
        qDebug("NETVInterfaceManager::NETVInterfaceManager with device : %p",device);

        //Create the handler
        m_handler = new NETVInterfaceHandler(device,args,this);


        //Register to message notify
        m_handler->registerObserver(this);


        //Create the scheduler
        m_scheduler = new NetworkScheduler(this);
    }
}

NETVInterfaceManager::~NETVInterfaceManager()
{

    //Remove all modules
    while(m_modules.size() > 0)
    {
        removeModule(m_modules.front());
    }

    if (m_scheduler)
    {
        delete m_scheduler;
        m_scheduler = NULL;
    }

    if (m_handler)
    {
        m_handler->unregisterObserver(this);

        delete m_handler;
        m_handler = NULL;
    }
}


void NETVInterfaceManager::sendAliveRequest()
{

    //qDebug("NETVInterfaceManager::sendAliveRequest()");

    //Building NETV request...
    NETV_MESSAGE message;

    message.msg_priority = NETV_PRIORITY_HIGHEST;
    message.msg_type = NETV_TYPE_EVENTS;
    message.msg_boot = 0;
    message.msg_cmd = NETV_EVENTS_CMD_ALIVE;

    //broadcast
    message.msg_dest = 0xFF;

    //This is a remote request
    message.msg_remote = 1;

    //Of zero size...
    message.msg_data_length = 8;

    //Sending to NETV bus...
    if (m_handler)
    {
        m_handler->pushNETVMessage(message);
    }
}

void NETVInterfaceManager::requestVariable(ModuleVariable *variable)
{

    //qDebug("NETVInterfaceManager::requestVariable(ModuleVariable *variable = %p)",variable);
    Q_ASSERT(variable);
    NetworkModule *module = getModule(variable->getDeviceID());
    Q_ASSERT(module);

    if (module->active() && variable->getMemType() < ModuleVariable::SCRIPT_VARIABLE && variable->getOffset() >= 0 )
    {

        //Building NETV request...
        NETV_MESSAGE message;

        message.msg_priority = NETV_PRIORITY_HIGHEST;
        message.msg_type = NETV_TYPE_REQUEST_DATA;

        //Requesting the right type of memory
        switch(variable->getMemType())
        {
        case ModuleVariable::RAM_VARIABLE:
            message.msg_boot = (NETV_REQUEST_RAM << 1) | (NETV_REQUEST_READ);
            break;

        case ModuleVariable::EEPROM_VARIABLE:
            message.msg_boot = (NETV_REQUEST_EEPROM << 1) | (NETV_REQUEST_READ);
            break;

        }

        message.msg_cmd = variable->getOffset();
        message.msg_dest = variable->getDeviceID();

        //This is a remote request
        message.msg_remote = 1;

        //Of the size of the variable...
        message.msg_data_length = variable->getLength();

        //Sending to NETV bus...
        if (m_handler)
        {
            m_handler->pushNETVMessage(message);
        }
    }
}

void NETVInterfaceManager::writeVariable(ModuleVariable *variable)
{

    //qDebug("NETVInterfaceManager::writeVariable(ModuleVariable *variable = %p)",variable);
    Q_ASSERT(variable);

    if (variable->getMemType() < ModuleVariable::SCRIPT_VARIABLE && variable->getOffset() >= 0)
    {

        //Building NETV request...
        NETV_MESSAGE message;

        message.msg_priority = NETV_PRIORITY_HIGHEST;
        message.msg_type = NETV_TYPE_REQUEST_DATA;

        //Requesting the right type of memory
        switch(variable->getMemType())
        {
        case ModuleVariable::RAM_VARIABLE:
            message.msg_boot = (NETV_REQUEST_RAM << 1) | (NETV_REQUEST_WRITE);
            break;

        case ModuleVariable::EEPROM_VARIABLE:
            message.msg_boot = (NETV_REQUEST_EEPROM << 1) | (NETV_REQUEST_WRITE);
            break;
        }

        message.msg_cmd = variable->getOffset();
        message.msg_dest = variable->getDeviceID();

        //This is a NOT remote request
        message.msg_remote = 0;

        //Of the size of the variable...
        message.msg_data_length = variable->getLength();

        //Set The data
        QByteArray array = variable->toByteArray();

        //Copy the data
        //TODO remove hard coded size here?
        for (int i =0; i < min(8,array.size()); i++)
        {
            message.msg_data[i] = array[i];
        }

        //Sending to NETV bus...
        if (m_handler)
        {
            m_handler->pushNETVMessage(message);
        }
    }
}

NetworkModule* NETVInterfaceManager::getModule(unsigned int deviceID)
{
    NetworkModule* module = NULL;

    for (unsigned int i = 0; i < m_modules.size(); i++)
    {
        ModuleConfiguration *conf = m_modules[i]->getConfiguration();

        if (conf->getDeviceID() == deviceID)
        {
            return m_modules[i];
        }
    }

    return module;
}

void  NETVInterfaceManager::notifyNETVMessage(const NETV_MESSAGE &msg)
{
    //qDebug("NETVInterfaceManager::notifyNETVMessage(const NETV_MESSAGE &msg)");
    //WATCH OUT, THIS IS CALLED FROM ANOTHER THREAD (NETVInterfaceHandler working thread)
    //Posting message to self (in the GUI thread)
    NETVMessageEvent *event = new NETVMessageEvent(msg);
    QCoreApplication::postEvent (this, event, Qt::HighEventPriority);
}

bool  NETVInterfaceManager::event ( QEvent * e )
{
    if (e->type() == QEvent::User)
    {
        //qDebug("NETVInterfaceManager::event %p QEvent::User",e);
        if (NETVMessageEvent *event = dynamic_cast<NETVMessageEvent*>(e))
        {
            processCANMessage(event->getMessage());
            return true;
        }
    }

    return QObject::event(e);
}

void NETVInterfaceManager::processCANMessage(const NETV_MESSAGE &msg)
{
    //qDebug("NETVInterfaceManager::processCANMessage(const NETV_MESSAGE &msg)");
    //NETVDevice::printMessage(msg,std::cout);
    if (!msg.msg_remote)
    {
        //WILL HANDLE ONLY REQUEST TYPE
        if (msg.msg_type == NETV_TYPE_REQUEST_DATA)
        {

            //NETVDevice::printMessage(msg,std::cout);

            //Let's see to which module it belongs...
            //Update the variable if it fits...
            for(unsigned int mod = 0; mod < m_modules.size(); mod++)
            {
                NetworkModule *module = m_modules[mod];

                ModuleConfiguration *conf = module->getConfiguration();
                Q_ASSERT(conf);

                if (conf->getDeviceID() == msg.msg_dest)
                {
                    for(int i =0; i < conf->size(); i++)
                    {
                        //Need to check the offset and the memory type
                        if ((*conf)[i]->getOffset() == msg.msg_cmd && (*conf)[i]->getMemType() == (ModuleVariable::VARIABLE_MEMORY_TYPE) (msg.msg_boot >> 1))
                        {
                            //FOUND IT
                            //UPDATING VALUE
                            //qDebug("found variable : ");
                            (*conf)[i]->setValue(msg.msg_data, msg.msg_data_length);
                            break;
                        }
                    }
                }
            }
        }//type == REQUEST_DATA
        else if (msg.msg_type == NETV_TYPE_EVENTS)
        {
            if (msg.msg_cmd == NETV_EVENTS_CMD_ALIVE && msg.msg_data_length == 8)
            {
                //qDebug("ALIVE REQUEST ANSWERED...");

                /*
                    msg.msg_data[0] = config->module_state;
                    msg.msg_data[1] = config->project_id;
                    msg.msg_data[2] = config->module_id;
                    msg.msg_data[3] = config->code_version;
                    msg.msg_data[4] = config->table_version;
                    msg.msg_data[5] = config->boot_delay;
                    msg.msg_data[6] = config->devid_low;
                    msg.msg_data[7] = config->devid_high;
                */

                int module_state = msg.msg_data[0];
                int project_id = msg.msg_data[1];
                int module_id = msg.msg_data[2];
                int code_version = msg.msg_data[3];
                int table_version = msg.msg_data[4];
                int boot_delay = msg.msg_data[5];
                int devID = (((int) msg.msg_data[7]) << 8) | (int) msg.msg_data[6];

                //Look for already existing modules...
                bool found = false;
                for (unsigned int mod = 0; mod < m_modules.size(); mod++)
                {
                    NetworkModule *module = m_modules[mod];

                    if (module->getConfiguration()->getDeviceID() == module_id)
                    {
                        found = true;

                        //Update time
                        module->setUpdateTime(QTime::currentTime());
                    }
                }

                if (!found)
                {
                    qDebug("Inserting new module %i",module_id);

                    //fill configuration information...
                    ModuleConfiguration conf(project_id, code_version, devID, module_state, table_version, module_id);

                    //Add module
                    addModule(new NetworkModule(conf,this));
                }
            }
        }//type == EVENTS
    } // NOT REMOTE
}

bool NETVInterfaceManager::addModule(NetworkModule *module)
{
    qDebug("NETVInterfaceManager::addModule(NetworkModule *module = %p)",module);

    if (module)
    {
        if (m_modules.contains(module))
        {
            return false;
        }
        else
        {
            m_modules.push_back(module);
            emit moduleAdded(module);

            //Connect user write
            connect(module,SIGNAL(variableWrite(ModuleVariable*)),this,SLOT(writeVariable(ModuleVariable*)));

            //Add module to scheduling
            if (m_scheduler)
            {
                m_scheduler->addModule(module);
            }
        }
    }

    return false;
}


bool NETVInterfaceManager::removeModule(NetworkModule *module)
{
    qDebug("NETVInterfaceManager::removeModule(NetworkModule *module = %p)",module);

    if (module)
    {
        if (m_modules.contains(module))
        {
            m_modules.removeAll(module);
            emit moduleRemoved(module);

            //Remove module from scheduler
            if (m_scheduler)
            {
                m_scheduler->removeModule(module);
            }

            delete module;
            return true;
        }
    }

    return false;
}

NETVInterfaceHandler* NETVInterfaceManager::getInterfaceHandler()
{
    return m_handler;
}

QList<NetworkModule*> NETVInterfaceManager::getModules()
{
    return m_modules;
}


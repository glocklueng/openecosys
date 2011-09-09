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

#include "dsPICBootloader.h"
#include "NetworkView.h"
#include "hexutils.h"
#include <QFileDialog>
#include <sstream>
#include "NetworkView.h"
#include "NETVMessageEvent.h"

//This will insert the plugin in the dictionary...
static int dspicbootloader_plugin_init = BasePlugin::registerPlugin("dsPICBootloader",new BasePlugin::PluginFactory<dsPICBootloader>());


dsPICBootloader::dsPICBootloader(NetworkView *view)
    : BasePlugin(view), m_interface(NULL), m_timer(NULL), m_module(NULL)
{

    //Create UI
    m_ui.setupUi(this);


    //Connect Signals
    connect(m_ui.m_loadHEXButton,SIGNAL(clicked()),this,SLOT(loadHEX()));
    connect(m_ui.m_uploadButton,SIGNAL(clicked()),this,SLOT(upload()));
    connect(m_ui.m_stopButton,SIGNAL(clicked()),this,SLOT(stop()));


    //Fill Combo
    QList<NETVInterfaceManager*> interfaceList = view->getInterfaceManagerList();
    for (unsigned int i= 0; i < interfaceList.size(); i++)
    {
        connect(interfaceList[i],SIGNAL(moduleAdded(NetworkModule*)),this,SLOT(moduleAdded(NetworkModule*)));
        connect(interfaceList[i],SIGNAL(moduleRemoved(NetworkModule*)),this,SLOT(moduleRemoved(NetworkModule*)));
    }

    rebuildCombo();




}

dsPICBootloader::~dsPICBootloader()
{
    if (m_interface)
    {
        m_interface->unregisterObserver(this);
        m_interface = NULL;
    }
}

void dsPICBootloader::init()
{

}

void dsPICBootloader::terminate()
{
    if (m_interface)
    {
        m_interface->unregisterObserver(this);
        m_interface = NULL;
    }

}

void dsPICBootloader::printMessage(const QString &message)
{
    m_ui.m_textEdit->append(message);
}

void dsPICBootloader::loadHEX()
{

    QString fileName = QFileDialog::getOpenFileName(this,
         tr("Open HEX File"), ".", tr("HEX Files (*.hex)"));

    if (fileName.size() > 0)
    {
        printMessage(QString("loading : ") + fileName);

        m_doc.load(fileName.toStdString());

        printMessage("Done!");

        printMessage("Document line size : " + QString::number(m_doc.size()));

        if (m_doc.validate())
        {
            m_doc.parse();
            ostringstream buffer;
            m_doc.print(buffer);
            printMessage(QString::fromAscii(buffer.str().c_str(),buffer.str().size()));
        } //document validate
        else
        {
            printMessage(fileName + "Checksum errors found, aborting");
        }
    } //filename validate
}


void dsPICBootloader::addEmergencyProgram(unsigned int moduleID)
{
    //Setup message
    NETV_MESSAGE message;
    message.msg_priority = 0;
    message.msg_cmd = NETV_EMERGENCY_CMD_PROGRAM;
    message.msg_data_length = 0;
    message.msg_type = NETV_TYPE_EMERGENCY;
    message.msg_boot = 0;
    message.msg_remote = 0;
    message.msg_dest = moduleID;

    //Add to queue
    m_msgQueue.push_back(message);
}


void dsPICBootloader::addSetBaseAddress(unsigned int moduleID, unsigned int address)
{
    //Setup message
    NETV_MESSAGE message;
    message.msg_priority = 0;
    message.msg_cmd = BOOTLOADER_SET_BASE_ADDR;
    message.msg_data_length = sizeof(unsigned int);
    message.msg_type = NETV_TYPE_BOOTLOADER;
    message.msg_boot = 0;
    message.msg_remote = 0;
    message.msg_dest = moduleID;

    //copy data
    memcpy(message.msg_data,(char*) &address, sizeof(unsigned int));

    //Add to queue
    m_msgQueue.push_back(message);
}


void dsPICBootloader::addResetCommand(unsigned int moduleID)
{
    NETV_MESSAGE message;
    message.msg_priority = 0;
    message.msg_cmd = BOOTLOADER_RESET;
    message.msg_data_length = 0;
    message.msg_type = NETV_TYPE_BOOTLOADER;
    message.msg_boot = 0;
    message.msg_remote = 0;
    message.msg_dest = moduleID;
    m_msgQueue.push_back(message);

}

void dsPICBootloader::addSetState(unsigned int moduleID, unsigned char state)
{
    NETV_MESSAGE message;
    message.msg_priority = 0;
    message.msg_cmd = BOOTLOADER_SET_STATE;
    message.msg_data_length = 1;
    message.msg_data[0] = state;
    message.msg_type = NETV_TYPE_BOOTLOADER;
    message.msg_boot = 0;
    message.msg_remote = 0;
    message.msg_dest = moduleID;
    m_msgQueue.push_back(message);
}

void dsPICBootloader::addWriteBootConfig(unsigned int moduleID)
{
    NETV_MESSAGE message;
    message.msg_priority = 0;
    message.msg_cmd = BOOTLOADER_WRITE_BOOTCONFIG;
    message.msg_data_length = 0;
    message.msg_type = NETV_TYPE_BOOTLOADER;
    message.msg_boot = 0;
    message.msg_remote = 0;
    message.msg_dest = moduleID;
    m_msgQueue.push_back(message);
}

void dsPICBootloader::addSendDataInc(unsigned int moduleID, QVector<unsigned char> &data)
{

    //Segmenting into 8 bytes data packets
    if (data.size() % 8 != 0)
    {
        printMessage(QString("addSendDataInc strange size :") + QString::number(data.size()));
    }


    for (unsigned int i = 0; i < data.size(); i+=8)
    {

        NETV_MESSAGE message;
        message.msg_priority = 0;
        message.msg_cmd = BOOTLOADER_WRITE_INC;
        message.msg_data_length = 8;
        memcpy(message.msg_data,&data.data()[i],8);
        message.msg_type = NETV_TYPE_BOOTLOADER;
        message.msg_boot = 0;
        message.msg_remote = 0;
        message.msg_dest = moduleID;
        m_msgQueue.push_back(message);
    }

}

void dsPICBootloader::generateMemoryMap(hexutils::hex32doc &doc)
{
    //document lines are ordered by address
    //Real addresses are already parsed correctly (high bytes) and stored into lines

    //Emtpy memory data
    m_memoryData.resize(0);

    //Go through all lines
    for (size_t i =0; i < doc.size(); i++)
    {

        hexutils::hex32line &line = doc.getLine(i);

        //We are only interested in data lines
        if (line.getType() == 0x00)
        {

            unsigned int addr = line.getAddress() * BOOT_LOADER_WORD_SIZE;


            if (addr < (BOOT_LOADER_ADDRESS * BOOT_LOADER_WORD_SIZE))
            {

                //Dynamically growing memory region
                if (m_memoryData.size() < (addr + line.getNbBytes()))
                {
                    m_memoryData.resize(addr + line.getNbBytes());
                }

                //Copy memory data
                for (unsigned int j = addr; j < (addr + line.getNbBytes());j++)
                {
                    m_memoryData[j] = line.getData()[j - addr];
                }
            }
            else
            {
                printMessage(QString("Flushing Line @0x") + QString::number(addr/BOOT_LOADER_WORD_SIZE,16));
            }

        }
    }

    if (m_memoryData.size() > 2)
    {
        //page size is 64 words * 2 bytes
        int modulo = m_memoryData.size() % (BOOT_LOADER_WORD_SIZE * BOOT_LOADER_PAGE_SIZE);

        if (modulo > 0)
        {
            m_memoryData.resize(m_memoryData.size() + ((BOOT_LOADER_WORD_SIZE * BOOT_LOADER_PAGE_SIZE) - modulo));
            printMessage(QString("Resizing memory map to fit page size :") + QString::number(m_memoryData.size()));
        }


        printMessage(QString("Memory Map ends @0x") + QString::number(m_memoryData.size() / BOOT_LOADER_WORD_SIZE,16));

        //Make sure the reset vector is correct
        //"Jump @0x9000"
        m_memoryData[0] = (unsigned char) (BOOT_LOADER_ADDRESS & 0x00FF);
        m_memoryData[1] = (unsigned char) (BOOT_LOADER_ADDRESS >> 8);
    }

}


void dsPICBootloader::generateMessageQueue(hexutils::hex32doc &doc, NetworkModule *module)
{
    if (module)
    {
        //Clear message queue
        m_msgQueue.clear();

        unsigned int moduleID = module->getConfiguration()->getDeviceID();

        //This will organize memory so that everything is aligned on FLASH pages
        generateMemoryMap(doc);

        //Ask module to switch back to bootloader
        addEmergencyProgram(moduleID);

        //Set base address to 0
        addSetBaseAddress(moduleID,0);

        //This will generate multiple can messages
        addSendDataInc(moduleID,m_memoryData);

        //Set module to normal mode
        addSetState(moduleID,NETV_NORMAL_MODE_ID);

        //Write config, will reboot in normal mode
        addWriteBootConfig(moduleID);

        //Reset when done!
        addResetCommand(moduleID);
    }
}

void dsPICBootloader::upload()
{
    printMessage("Uploading...");

    //get selected module
    QPoint data = m_ui.m_moduleSelectionCombo->itemData(m_ui.m_moduleSelectionCombo->currentIndex()).toPoint();

    NETVInterfaceManager *manager = m_view->getInterfaceManagerList()[data.x()];
    m_module = manager->getModule(data.y());

    if (m_module)
    {
        generateMessageQueue(m_doc,m_module);
        connect(m_module,SIGNAL(moduleDestroyed()),this,SLOT(moduleDestroyed(NetworkModule*)));
    }


    if (m_msgQueue.size())
    {
        //get selected module
        unsigned int index = m_ui.m_moduleSelectionCombo->currentIndex();

        //Do you get the point! ;)
        QPoint data = m_ui.m_moduleSelectionCombo->itemData(index).toPoint();



        m_interface = m_view->getInterfaceManagerList()[data.x()]->getInterfaceHandler();

        //Register ourself to messages...
        if (m_interface)
        {
            //Start timer
            m_elapsed.start();


            m_interface->registerObserver(this);


            printMessage("Starting upload...");

            //Reset ProgressBar
            m_ui.m_progressBar->setMinimum(0);
            //first message does not count (EMERGENCY_PROGRAM)
            m_ui.m_progressBar->setMaximum(m_msgQueue.size() - 1);
            m_ui.m_progressBar->setValue(0);


            //Send first message
            //This should be the EMERGENCY_PROGRAM_COMMAND
            //We will then wait for the module to be in the bootloader
            m_interface->pushNETVMessage(m_msgQueue.front());


            m_timer = new QTimer(this);
            connect(m_timer,SIGNAL(timeout()),this,SLOT(timeout()));
            m_timer->start(1000); //1000ms

        }

    }
    else
    {
        printMessage("Nothing to upload...");
    }
}

void dsPICBootloader::notifyNETVMessage(const NETV_MESSAGE &msg)
{
    //THIS IS CALLED FROM ANOTHER THREAD, BE CAREFUL
    QCoreApplication::postEvent(this, new NETVMessageEvent(msg),Qt::HighEventPriority);
}

bool  dsPICBootloader::event ( QEvent * e )
{
    if (e->type() == QEvent::User)
    {
        //qDebug("NETVInterfaceManager::event %p QEvent::User",e);
        if (NETVMessageEvent *event = dynamic_cast<NETVMessageEvent*>(e))
        {
            NETV_MESSAGE msg = event->getMessage();
            //printMessage(QString("notify type ") + QString::number(msg.msg_type));

            //Bootloader feedback...
            if (msg.msg_type == NETV_TYPE_BOOTLOADER && m_interface)
            {

                unsigned int index = m_ui.m_moduleSelectionCombo->currentIndex();

                //Do you get the point! ;)
                QPoint data = m_ui.m_moduleSelectionCombo->itemData(index).toPoint();


                //Feedback from the right module
                if (msg.msg_dest == data.y())
                {
                    //Validate feedback
                    if (m_msgQueue.front() == msg)
                    {
                        m_msgQueue.pop_front();

                        //Increment progressbar
                        m_ui.m_progressBar->setValue(m_ui.m_progressBar->value() + 1);

                        //printMessage(QString("Queue size : ") + QString::number(m_msgQueue.size()));

                        if (m_msgQueue.size() > 0)
                        {
                            //Send next
                            m_interface->pushNETVMessage(m_msgQueue.front());

                            //Resetart timer
                            m_elapsed.start();
                        }
                        else
                        {
                            printMessage("Done programming.");
                            stop();
                        }

                    }
                    else
                    {
                        //Re-send message
                        //Abort?
                        printMessage("Transfer error, stopping");
                        stop();
                    }
                }

            }

            //We have processed this event
            return true;
        }
    }

    return QObject::event(e);
}

void dsPICBootloader::timeout()
{
    if (m_module && m_interface)
    {
        //Boot mode?
        if (m_module->getConfiguration()->getModuleState() == NETV_BOOT_MODE_ID)
        {
            //Elapsed time
            if(m_elapsed.elapsed() > 100) //100ms
            {

                printMessage("Timeout...");
                if (m_msgQueue.front().msg_type != NETV_TYPE_BOOTLOADER)
                {
                    m_msgQueue.pop_front();
                    printMessage("Ready!");
                }

                printMessage(QString("Queue size : ") + QString::number(m_msgQueue.size()));
                //(Re)Send message on top of the stack
                m_interface->pushNETVMessage(m_msgQueue.front());
            }
        }
        else
        {
            printMessage(QString("Module not ready... ") + QString::number(m_module->getConfiguration()->getModuleState()));

            if (m_elapsed.elapsed() > 10000)
            {
                //abort transfer...
                stop();
            }
        }
    }
    else
    {
        stop();
    }
}

void dsPICBootloader::stop()
{
    printMessage("Stopping...");

    if (m_timer)
    {
        delete m_timer;
        m_timer = NULL;
    }

    if (m_interface)
    {
        m_interface->unregisterObserver(this);
        m_interface = NULL;
    }

    m_ui.m_progressBar->setValue(0);
    m_msgQueue.clear();
    m_memoryData.resize(0);
    m_module = NULL;
}

void dsPICBootloader::moduleAdded(NetworkModule *module)
{
    rebuildCombo();
}

void dsPICBootloader::moduleRemoved(NetworkModule *module)
{
    if (m_module == module)
    {
        m_module = NULL;
        stop();
    }

    rebuildCombo();
}

void dsPICBootloader::rebuildCombo()
{
    //Clear combo first
    m_ui.m_moduleSelectionCombo->clear();

    //Fill Combo
    QList<NETVInterfaceManager*> interfaceList = m_view->getInterfaceManagerList();

    for (unsigned int i= 0; i < interfaceList.size(); i++)
    {
        QList<NetworkModule*> allModules = interfaceList[i]->getModules();

        for (unsigned j = 0; j < allModules.size(); j++)
        {
            NetworkModule* mod = allModules[j];

            //Add combo item
            m_ui.m_moduleSelectionCombo->addItem(QString("Module ") + QString::number(mod->getConfiguration()->getDeviceID()),QPoint(i,mod->getConfiguration()->getDeviceID()));
        }
    }
}

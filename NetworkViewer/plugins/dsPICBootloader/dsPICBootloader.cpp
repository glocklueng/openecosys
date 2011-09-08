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
    : BasePlugin(view), m_interface(NULL), m_timer(NULL)
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
        QList<NetworkModule*> allModules = interfaceList[i]->getModules();

        for (unsigned j = 0; j < allModules.size(); j++)
        {
            NetworkModule* mod = allModules[j];

            //Add combo item
            m_ui.m_moduleSelectionCombo->addItem(QString("Module ") + QString::number(mod->getConfiguration()->getDeviceID()),QPoint(i,mod->getConfiguration()->getDeviceID()));
        }
    }
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
    hexutils::hex32doc doc;

    QString fileName = QFileDialog::getOpenFileName(this,
         tr("Open HEX File"), ".", tr("HEX Files (*.hex)"));

    if (fileName.size() > 0)
    {
        printMessage(QString("loading : ") + fileName);

        doc.load(fileName.toStdString());

        printMessage("Done!");

        printMessage("Document line size : " + QString::number(doc.size()));

        if (doc.validate())
        {
            doc.parse();
            ostringstream buffer;
            doc.print(buffer);
            printMessage(QString::fromAscii(buffer.str().c_str(),buffer.str().size()));
            generateMessageQueue(doc,NULL);
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


void dsPICBootloader::addSendDataInc(unsigned int moduleID, std::vector<unsigned char> &data)
{

    //Segmenting into 8 bytes data packets
    for (unsigned int i = 0; i < data.size(); i+=8)
    {

        NETV_MESSAGE message;
        message.msg_priority = 0;
        message.msg_cmd = BOOTLOADER_WRITE_INC;
        message.msg_data_length = std::min((unsigned int)data.size() - i,(unsigned int)8);
        memcpy(message.msg_data,&data[i], std::min((unsigned int)data.size() - i,(unsigned int)8));
        message.msg_type = NETV_TYPE_BOOTLOADER;
        message.msg_boot = 0;
        message.msg_remote = 0;
        message.msg_dest = moduleID;
        m_msgQueue.push_back(message);
    }

}


void dsPICBootloader::generateMessageQueue(hexutils::hex32doc &doc, NetworkModule *module)
{
    //This will generate the sequence of message to be sent to the module
    unsigned int moduleID = 0;
    unsigned int baseAddress = 0;
    bool highAddressChanged = false;

    //ASK MODULE TO SWITCH BACK TO BOOTLOADER
    addEmergencyProgram(moduleID);

    //SET BASE ADDRESS TO 0
    addSetBaseAddress(moduleID,baseAddress);

    //PROCESS HEX DATA
    for (size_t i = 0; i < doc.size(); i++)
    {
        hexutils::hex32line &line = doc.getLine(i);

        if (line.getType() == 0x00)
        {
            //update address
            unsigned int addr = line.getAddress();

            baseAddress &= 0xFFFF0000; //Clear old LSB
            baseAddress |= (addr & 0x0000FFFF); //Set new LSB

            //Data line
            if (highAddressChanged)
            {
                printMessage("Set base address : "+ QString::number(baseAddress,16).toUpper());


                unsigned int modulo = baseAddress % 64;
                //Calculate base page address
                if (modulo != 0)
                {
                    printMessage("Should load page at address : " + QString::number(baseAddress - modulo,16) + "with offset " + QString::number(modulo));
                }



                //Send base address first
                addSetBaseAddress(moduleID,baseAddress);
                highAddressChanged = false;
            }

            //Send data line
            addSendDataInc(moduleID,line.getData());

        }
        else if (line.getType() == 0x01)
        {
            //end document
            break;
        }
        else if (line.getType() == 0x04)
        {
            if (line.getNbBytes() == 2)
            {
                highAddressChanged = true;

                //Get the MSB
                unsigned int msb = 0;

                msb = ((unsigned int) line.getData()[0]) << 24;
                msb |= ((unsigned int) line.getData()[1]) << 16;

                //Update base address
                //Must be divided by 2 for real PIC address
                baseAddress = msb / 2; //set new MSB
            }
        }
    }

    //RESET WHEN DONE!
    addResetCommand(moduleID);
}

void dsPICBootloader::upload()
{
    printMessage("Uploading...");


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
                 printMessage("Feedback!!!");
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
    //Which module are we programming ?
    //Is it ready?
    QPoint data = m_ui.m_moduleSelectionCombo->itemData(m_ui.m_moduleSelectionCombo->currentIndex()).toPoint();

    NETVInterfaceManager *manager = m_view->getInterfaceManagerList()[data.x()];
    NetworkModule *module = manager->getModule(data.y());

    if (module)
    {
        //Boot mode?
        if (module->getConfiguration()->getModuleState() == NETV_BOOT_MODE_ID)
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
            printMessage(QString("Module not ready... ") + QString::number(module->getConfiguration()->getModuleState()));

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
}

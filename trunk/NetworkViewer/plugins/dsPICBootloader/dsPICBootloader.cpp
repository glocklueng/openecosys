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

//This will insert the plugin in the dictionary...
static int dspicbootloader_plugin_init = BasePlugin::registerPlugin("dsPICBootloader",new BasePlugin::PluginFactory<dsPICBootloader>());


dsPICBootloader::dsPICBootloader(NetworkView *view)
    : BasePlugin(view)
{

    //Create UI
    m_ui.setupUi(this);


    //Connect Signals
    connect(m_ui.m_loadHEXButton,SIGNAL(clicked()),this,SLOT(loadHEX()));

}

void dsPICBootloader::init()
{

}

void dsPICBootloader::terminate()
{

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



void dsPICBootloader::generateMessageQueue(hexutils::hex32doc &doc, NetworkModule *module)
{
    //This will generate the sequence of message to be sent to the module
    unsigned int moduleID = 0;
    unsigned int baseAddress = 0;
    bool highAddressChanged = false;


    //RESET FIRST
    addResetCommand(moduleID);

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

                //Send base address first
                addSetBaseAddress(moduleID,baseAddress);
                highAddressChanged = false;
            }

            //Send data line


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
}

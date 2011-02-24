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

#include "DeviceSelectorDialog.h"
#include "NetworkView.h"
#include "CANDevice.h"

DeviceSelectorDialog::DeviceSelectorDialog(NetworkView *view)
    : m_view(view), m_factory(NULL)
{

    qDebug("DeviceSelectorDialog");
    setupUi(this);

    //Look for devices and add them to the combo box
    QStringList devList = CANDevice::deviceList();

    for (int i = 0; i < devList.size(); i++)
    {
        comboBox->addItem(devList[i]);
	comboActivated(i);
    }


    connect(comboBox,SIGNAL(activated(int)),this,SLOT(comboActivated(int)));
    connect(m_configureButton,SIGNAL(clicked()),this,SLOT(configureButtonClicked()));


    qDebug("DeviceSelectorDialog done");
}


QString DeviceSelectorDialog::selectedDevice()
{
    return comboBox->currentText();
}

void DeviceSelectorDialog::comboActivated(int index)
{
    //update args and documentation
    m_factory= CANDevice::getFactoryNamed(comboBox->currentText());

    if (m_factory)
    {
        //update default args
        lineEditArgs->setText(m_factory->getDefaultArgs());

        //update documentation
        textEditDoc->setText(m_factory->getDocumentation());
    }
    else
    {
        lineEditArgs->setText("");
        textEditDoc->setText("");
    }
}

QString DeviceSelectorDialog::args()
{
    return lineEditArgs->text();
}


QString DeviceSelectorDialog::documentation()
{
    return textEditDoc->toPlainText();
}

CANDevice::BaseDeviceFactory* DeviceSelectorDialog::getFactory()
{
    return m_factory;
}

void DeviceSelectorDialog::configureButtonClicked()
{

    qDebug("DeviceSelectorDialog::configureButtonClicked()");

    if (m_factory)
    {
        QString result = m_factory->configure();

        lineEditArgs->setText(result);
    }
}



/**
     Copyright (C) 2009-2010 IntRoLab
     http://introlab.gel.usherbrooke.ca
     Dominic Letourneau, ing. M.Sc.A.
     Dominic.Letourneau@USherbrooke.ca
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




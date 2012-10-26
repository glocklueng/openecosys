/**
     Copyright (C) 2009-2012 IntRoLab
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

#include "SerialPortSelectionDialog.h"
#include <qextserialenumerator.h>
#include <QList>


SerialPortSelectionDialog::SerialPortSelectionDialog(QWidget *parent)
    : QDialog(parent), m_currentPortIndex(-1), m_currentBaudRateIndex(0)
{
    resize(640,480);
    setupUi(this);

    connect(m_portComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(portIndexChanged(int)));
    connect(m_baudComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(baudRateIndexChanged(int)));

    //Fill combo box with available ports
    QList<QextPortInfo> ports = QextSerialEnumerator::getPorts();

    for (unsigned int i = 0; i < ports.size(); i++)
    {
        if (i == 0)
        {
            m_currentPortIndex = 0;

            //Update info
            m_textEdit->append("port name:" + ports.at(i).portName);
            m_textEdit->append("friendly name:" + ports.at(i).friendName);
            m_textEdit->append("physical name:" +  ports.at(i).physName);
            m_textEdit->append("enumerator name:" + ports.at(i).enumName);
            m_textEdit->append("vendor ID:" + QString::number(ports.at(i).vendorID, 16));
            m_textEdit->append("product ID:" + QString::number(ports.at(i).productID, 16));
            m_textEdit->append("===================================");
        }

        //Add port to combo list
        m_portComboBox->addItem(ports.at(i).portName);
    }

    //Set Combo box @ 9600 baud
    int index = m_baudComboBox->findText("9600");
    m_baudComboBox->setCurrentIndex(index);
}

QString SerialPortSelectionDialog::getSerialPortString()
{
    if (m_currentPortIndex >= 0)
    {
        return m_portComboBox->itemText(m_currentPortIndex);
    }

    return QString();
}

QString SerialPortSelectionDialog::getBaudRateString()
{
    return m_baudComboBox->itemText(m_currentBaudRateIndex);
}


void SerialPortSelectionDialog::portIndexChanged ( int index )
{
    m_currentPortIndex = index;

    //Fill combo box with available ports
    QList<QextPortInfo> ports = QextSerialEnumerator::getPorts();


    if (index < ports.size())
    {
        m_textEdit->clear();

        //Update info
        m_textEdit->append("port name:" + ports.at(index).portName);
        m_textEdit->append("friendly name:" + ports.at(index).friendName);
        m_textEdit->append("physical name:" +  ports.at(index).physName);
        m_textEdit->append("enumerator name:" + ports.at(index).enumName);
        m_textEdit->append("vendor ID:" + QString::number(ports.at(index).vendorID, 16));
        m_textEdit->append("product ID:" + QString::number(ports.at(index).productID, 16));
        m_textEdit->append("===================================");

    }

}

void SerialPortSelectionDialog::baudRateIndexChanged ( int index )
{
    m_currentBaudRateIndex = index;
}

bool SerialPortSelectionDialog::getDebugStatus()
{
    return m_debugCheckbox->isChecked();
}

int SerialPortSelectionDialog::getDelay()
{
    return m_delaySpinBox->value();
}

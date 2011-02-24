#include "SerialEmulatorConfigure.h"

#include <qextserialenumerator.h>
#include <QList>


SerialEmulatorConfigure::SerialEmulatorConfigure(QWidget *parent)
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



}

QString SerialEmulatorConfigure::getSerialPortString()
{
    if (m_currentPortIndex >= 0)
    {
        return m_portComboBox->itemText(m_currentPortIndex);
    }

    return QString();
}

QString SerialEmulatorConfigure::getBaudRateString()
{
    return m_baudComboBox->itemText(m_currentBaudRateIndex);
}


void SerialEmulatorConfigure::portIndexChanged ( int index )
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

void SerialEmulatorConfigure::baudRateIndexChanged ( int index )
{
    m_currentBaudRateIndex = index;
}

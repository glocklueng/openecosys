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
#include "ModuleVariable.h"
#include <QBuffer>
#include <sstream>

//This constructor is private
ModuleVariable::ModuleVariable(QObject *parent)
    : QObject(parent), m_type(INVALID), m_name("Invalid"), m_memType(RAM_VARIABLE)
    , m_offset(-1), m_description("Invalid variable"), m_version(0), m_deviceID(-1), m_interfaceID(-1), m_activated(true)
{

}



ModuleVariable::ModuleVariable(VARIABLE_TYPE _type, const QString &_name, VARIABLE_MEMORY_TYPE _memType, int _offset, const QString &_description , QObject *parent)
    : QObject(parent), m_type(_type), m_name(_name), m_memType(_memType)
    , m_offset(_offset), m_description(_description), m_activated(true)
{
    //Set version to zero
    m_version = 0;

    //Set deviceID to -1
    m_deviceID = -1;

    //Set interfaceID to -1
    m_interfaceID = -1;

    setProperty("name",QString("ModuleVariable:") + QString(m_name));
}


ModuleVariable& ModuleVariable::invalid()
{
    static ModuleVariable invalidVariable;
    return invalidVariable;
}

ModuleVariable::ModuleVariable(const ModuleVariable &variable)
    : m_type(INVALID), m_name("Invalid"), m_offset(-1), m_description("Invalid variable"), m_version(0),  m_deviceID(-1), m_activated(true)
{
    m_type = variable.m_type;
    m_name = variable.m_name;
    m_memType = variable.m_memType;
    m_offset = variable.m_offset;
    m_description = variable.m_description;
    m_value = variable.m_value;
    m_version = variable.m_version;
    m_deviceID = variable.m_deviceID;
    m_interfaceID = variable.m_interfaceID;
    m_activated = variable.m_activated;

    setProperty("name",QString("ModuleVariable:") + QString(m_name));
}

ModuleVariable::ModuleVariable(QDomElement &element)
    : m_type(INVALID), m_name("Invalid"), m_memType(RAM_VARIABLE), m_offset(-1), m_description("Invalid variable")
    ,  m_version(0), m_deviceID(-1), m_interfaceID(-1), m_activated(true)
{

    loadXML(element);

    setProperty("name",QString("ModuleVariable:") + QString(m_name));
}

ModuleVariable::~ModuleVariable()
{
    emit aboutToDestroy(this);
}

bool ModuleVariable::loadXML(QDomElement &element)
{
    if(element.tagName() == "ModuleVariable")
    {
        //Get all the content...
        /*
			VARIABLE_TYPE m_type;
			QString m_name;
			unsigned int m_offset;
			QString m_description;
			QVariant m_value;
			unsigned long m_version;
			int m_deviceID;
		*/

        m_type = ModuleVariable::stringToType(element.attributeNode("type").value());
        m_name = element.attributeNode("name").value();

        //For backward compatibility, we consider variable to be of RAM type if not specified...
        if (element.hasAttribute("memType"))
        {
            m_memType = (VARIABLE_MEMORY_TYPE) element.attributeNode("memType").value().toInt();
        }
        else
        {
            m_memType = RAM_VARIABLE;
        }


        m_offset = element.attributeNode("offset").value().toInt();
        m_description = element.attributeNode("description").value();

        //Default value (from string)...
        if (element.attributeNode("value").value().size())
        {
            setValue(element.attributeNode("value").value());
        }




        return true;
    }
    else
    {
        qDebug() << "ModuleVariable::loadXML() invalid tag name:" << element.tagName();
        return false;
    }

}







ModuleVariable& ModuleVariable::operator= (const ModuleVariable &variable)
                                          {
    m_type = variable.m_type;
    m_name = variable.m_name;
    m_memType = variable.m_memType;
    m_offset = variable.m_offset;
    m_description = variable.m_description;
    m_value = variable.m_value;
    m_deviceID = variable.m_deviceID;
    m_interfaceID = variable.m_interfaceID;
    m_version = variable.m_version;

    setProperty("name",QString("ModuleVariable:") + QString(m_name));

    emit valueChanged(this);

    return *this;
}

bool ModuleVariable::operator== (const ModuleVariable &variable) const
{
    if (m_name == variable.m_name &&
        m_memType == variable.m_memType &&
        m_version == variable.m_version &&
        m_deviceID == variable.m_deviceID &&
        m_interfaceID == variable.m_interfaceID)
    {
        return true;
    }
    else
    {
        return false;
    }
}


void ModuleVariable::saveXML(QDomDocument &document, QDomElement &parentElement)
{
    QDomElement variableElement = document.createElement("ModuleVariable");

    variableElement.setAttribute("type",typeToString(m_type));
    variableElement.setAttribute("name",m_name);
    variableElement.setAttribute("memType", QString::number(m_memType));
    variableElement.setAttribute("offset",QString::number(m_offset));
    variableElement.setAttribute("description",m_description);
    variableElement.setAttribute("value",m_value.toString());

    //deviceID not required...
    //variableElement.setAttribute("deviceID",QString::number(m_deviceID));

    //interfaceID not required...
    //variableElement.setAttribute("interfaceID",QString::number(m_interfaceID));

    //version not required...
    //variableElement.setAttribute("version",QString::number(m_version));



    parentElement.appendChild(variableElement);
}

QString ModuleVariable::typeToString(VARIABLE_TYPE type)
{
    QString typeString;

    switch(type)
    {
    case DOUBLE:
        typeString="double";
        break;

    case FLOAT:
        typeString="float";
        break;

    case SINT32:
        typeString="sint32";
        break;

    case UINT32:
        typeString="uint32";
        break;

    case SINT16:
        typeString="sint16";
        break;

    case UINT16:
        typeString="uint16";
        break;

    case SINT8:
        typeString="sint8";
        break;

    case UINT8:
        typeString="uint8";
        break;

    default:
        qDebug("Unhandled type : %i",type);
        typeString="Unknown";
        break;

    }

    return typeString;
}

ModuleVariable::VARIABLE_TYPE ModuleVariable::stringToType(const QString &typeString)
{
    if (typeString == "double")
    {
        return DOUBLE;
    }
    else if (typeString == "float")
    {
        return FLOAT;
    }
    else if (typeString == "sint32")
    {
        return SINT32;
    }
    else if (typeString == "uint32")
    {
        return UINT32;
    }
    else if (typeString == "sint16")
    {
        return SINT16;
    }
    else if (typeString == "uint16")
    {
        return UINT16;
    }
    else if (typeString == "sint8")
    {
        return SINT8;
    }
    else if (typeString == "uint8")
    {
        return UINT8;
    }
    else
    {
        qDebug() << "Invalid type :" << typeString;
        return INVALID;
    }
}


void ModuleVariable::setValue(float value, bool userUpdate)
{
    m_value = QVariant(value);
    m_version++;
    emit valueChanged(this);
    if (userUpdate)
    {
        //emit user signal
        emit userChanged(this);
    }
}

void ModuleVariable::setValue(double value, bool userUpdate)
{
    m_value = QVariant(value);
    m_version++;
    emit valueChanged(this);
    if (userUpdate)
    {
        //emit user signal
        emit userChanged(this);
    }
}

void ModuleVariable::setValue (int value, bool userUpdate)
{
    m_value = QVariant(value);
    m_version++;
    emit valueChanged(this);
    if (userUpdate)
    {
        //emit user signal
        emit userChanged(this);
    }
}

void ModuleVariable::setUserValue(int value)
{
    setValue(value,true);
}

void ModuleVariable::setUserValue(bool value)
{
    setValue((unsigned char) value,true);
}

void ModuleVariable::setUserValue(QVariant value)
{
    setValue(value,true);
}

void ModuleVariable::setUserValue(double value)
{
    setValue(value,true);
}

void ModuleVariable::setValue (unsigned int value, bool userUpdate)
{
    m_value = QVariant(value);
    m_version++;
    emit valueChanged(this);
    if (userUpdate)
    {
        //emit user signal
        emit userChanged(this);
    }
}

void ModuleVariable::setValue (short value, bool userUpdate)
{
    m_value = QVariant(value);
    m_version++;
    emit valueChanged(this);
    if (userUpdate)
    {
        //emit user signal
        emit userChanged(this);
    }
}

void ModuleVariable::setValue (unsigned short value, bool userUpdate)
{
    m_value = QVariant(value);
    m_version++;
    emit valueChanged(this);
    if (userUpdate)
    {
        //emit user signal
        emit userChanged(this);
    }
}

void ModuleVariable::setValue (char value, bool userUpdate)
{
    m_value = QVariant(value);
    m_version++;
    emit valueChanged(this);
    if (userUpdate)
    {
        //emit user signal
        emit userChanged(this);
    }
}

void ModuleVariable::setValue (unsigned char value, bool userUpdate)
{
    m_value = QVariant(value);
    m_version++;
    emit valueChanged(this);
    if (userUpdate)
    {
        //emit user signal
        emit userChanged(this);
    }
}


void ModuleVariable::setValue(QVariant value, bool userUpdate)
{
    m_value = value;
    m_version++;
    emit valueChanged(this);
    if (userUpdate)
    {
        //emit user signal
        emit userChanged(this);
    }
}

void ModuleVariable::setValue (const QString &value, bool userUpdate)
{
    bool ok = false;

    switch(m_type)
    {
    case DOUBLE:
        //TODO DO SOMETHING WITH OK!
        setValue(value.toDouble(&ok));
        break;

    case FLOAT:
        //TODO DO SOMETHING WITH OK!
        setValue(value.toFloat(&ok));
        break;

    case SINT32:
        //TODO DO SOMETHING WITH OK!
        setValue(value.toInt(&ok));
        break;

    case UINT32:
        //TODO DO SOMETHING WITH OK!
        setValue(value.toUInt(&ok));
        break;

    case SINT16:
        //TODO DO SOMETHING WITH OK!
        setValue(value.toShort(&ok));
        break;
    case UINT16:
        //TODO DO SOMETHING WITH OK!
        setValue(value.toUShort(&ok));
        break;

    case SINT8:
        //TODO DO SOMETHING WITH OK!
        setValue((char) value.toShort(&ok));
        break;

    case UINT8:
        //TODO DO SOMETHING WITH OK!
        setValue((unsigned char) value.toUShort(&ok));
        break;

    case INVALID:
        qDebug("Trying to set a value to invalid variable");
        break;

    }

    if (!ok)
    {
        qDebug() << "Invalid conversion from QString to " << typeToString(m_type) << " String: " << value;
    }
    else
    {
        if (userUpdate)
        {
            //emit user signal
            emit userChanged(this);
        }
    }

}


void ModuleVariable::setValue(const unsigned char* data, int size, bool userUpdate)
{
    if (size == getLength())
    {
        std::stringstream myStream(std::string((char*)data,size));

        switch(m_type)
        {
        case DOUBLE:
            {
                double value;
                myStream.read((char*) &value,sizeof(double));
                setValue(value,userUpdate);
            }
            break;

        case FLOAT:
            {
                float value;
                myStream.read((char*) &value,sizeof(float));
                setValue(value,userUpdate);
            }
            break;

        case SINT32:
            {
                int value;
                myStream.read((char*) &value,sizeof(int));
                setValue(value,userUpdate);
            }
            break;

        case UINT32:
            {
                unsigned int value;
                myStream.read((char*) &value,sizeof(unsigned int));
                setValue(value,userUpdate);
            }
            break;

        case SINT16:
            {
                short value;
                myStream.read((char*) &value,sizeof(short));
                setValue(value,userUpdate);
            }

            break;

        case UINT16:
            {
                unsigned short value;
                myStream.read((char*) &value,sizeof(unsigned short));
                setValue(value,userUpdate);
            }
            break;

        case SINT8:
            {
                char value;
                myStream.read((char*) &value,sizeof(char));
                setValue(value,userUpdate);
            }
            break;

        case UINT8:
            {
                unsigned char value;
                myStream.read((char*) &value,sizeof(unsigned char));
                setValue(value,userUpdate);
            }
            break;

        case INVALID:
            qDebug("Trying to set a value to invalid variable");
            break;

        }
    }
    else
    {
        qDebug("Invalid length %i for variable %s",size,getName().toStdString().c_str());
    }
}


QVariant ModuleVariable::getValue() const
{
    return m_value;
}

ModuleVariable::VARIABLE_TYPE ModuleVariable::getType() const
{
    return m_type;
}

ModuleVariable::VARIABLE_MEMORY_TYPE ModuleVariable::getMemType() const
{
    return m_memType;
}

QString ModuleVariable::getName() const
{
    return m_name;
}

int ModuleVariable::getOffset() const
{
    return m_offset;
}

QString ModuleVariable::getDescription() const
{
    return m_description;
}

unsigned long ModuleVariable::getVersion() const
{
    return m_version;
}


void ModuleVariable::setDeviceID(int id)
{
    m_deviceID = id;
}

int ModuleVariable::getDeviceID() const
{
    return m_deviceID;
}

void ModuleVariable::setInterfaceID(int id)
{
    m_interfaceID = id;
}

int ModuleVariable::getInterfaceID() const
{
    return m_interfaceID;
}


int ModuleVariable::getLength() const
{
    int size = 0;

    switch(m_type)
    {
    case DOUBLE:
        size = 8;
        break;

    case FLOAT:
        size = 4;
        break;

    case SINT32:
        size = 4;
        break;

    case UINT32:
        size = 4;
        break;

    case SINT16:
        size = 2;
        break;

    case UINT16:
        size = 2;
        break;

    case SINT8:
        size = 1;
        break;

    case UINT8:
        size = 1;
        break;

    case INVALID:
        qDebug("Trying to set a value to invalid variable");
        break;

    }

    return size;
}

QByteArray ModuleVariable::toByteArray() const
{
    QBuffer buffer;
    buffer.open(QBuffer::WriteOnly);

    switch(m_type)
    {
    case DOUBLE:
        {
            double myValue = m_value.toDouble();
            buffer.write((char*) &myValue,sizeof(double));
        }
        break;

    case FLOAT:
        {
            float myValue = (float) m_value.toDouble();
            buffer.write((char*) &myValue,sizeof(float));
        }
        break;

    case SINT32:
        {
            int myValue = m_value.toInt();
            buffer.write((char*) &myValue,sizeof(int));
        }

        break;

    case UINT32:
        {
            unsigned int myValue = m_value.toUInt();
            buffer.write((char*) &myValue,sizeof(unsigned int));
        }
        break;

    case SINT16:
        {
            short myValue = (short) m_value.toInt();
            buffer.write((char*) &myValue,sizeof(short));
        }
        break;

    case UINT16:
        {
            unsigned short myValue = (unsigned short) m_value.toUInt();
            buffer.write((char*) &myValue,sizeof(unsigned short));
        }
        break;

    case SINT8:
        {
            char myValue = (char) m_value.toInt();
            buffer.write((char*) &myValue,sizeof(char));
        }
        break;

    case UINT8:
        {
            unsigned char myValue = (unsigned char) m_value.toUInt();
            buffer.write((char*) &myValue,sizeof(unsigned char));
        }
        break;

    case INVALID:
        qDebug("Trying to set a value to invalid variable");
        break;
    }


    //Validation of the code...
    Q_ASSERT(buffer.data().size() == getLength());


    return buffer.data();

}

void ModuleVariable::setActivated(bool activated)
{
    if (activated != m_activated)
    {
        m_activated = activated;
        emit variableActivated(m_activated,this);
    }
}

bool ModuleVariable::getActivated() const
{
    return m_activated;
}


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

#ifndef _MODULE_VARIABLE_H_
#define _MODULE_VARIABLE_H_

#include <QObject>
#include <QList>
#include <QtDebug>
#include <QDomDocument>
#include <QFile>
#include <QVariant>
#include <QByteArray>
#include <QDateTime>


/**
  Data structure that holds the variable information.
  \author Dominic Letourneau
*/
class ModuleVariable : public QObject 
{
	
	Q_OBJECT;
	
public:
	
        /**
            Supported variable types
        */
	typedef enum {DOUBLE, FLOAT, SINT32, UINT32, SINT16, UINT16, SINT8, UINT8, INVALID} VARIABLE_TYPE ; 

        /**
            Variable can be either stored into RAM or EEPROM
        */
        typedef enum {RAM_VARIABLE,EEPROM_VARIABLE, SCRIPT_VARIABLE} VARIABLE_MEMORY_TYPE;
	
        /**
            Constructor
            \param _type The type of variable \ref VARIABLE_TYPE
            \param _name The name of the variable
            \param memType The memory type of the vafriable \ref VARIABLE_MEMORY_TYPE
            \param _offset The memory offset in the remote controller
            \param _description The description of the variable
            \param parent The parent object, defaults to NULL
        */
        ModuleVariable(VARIABLE_TYPE _type, const QString &_name, VARIABLE_MEMORY_TYPE memType, int _offset = -1, const QString &_description = "", QObject *parent = NULL);

        /**
            Copy Constructor
            \param variable The variable to copy
        */
        ModuleVariable(const ModuleVariable &variable);

        /**
            Constructor from XML definition
            \param element The QDomElement to read from
        */
        ModuleVariable(QDomElement &element);

        ///Desctructor
	virtual ~ModuleVariable();

        ///Returns invalid variable
	static ModuleVariable& invalid();
	

        /**
            operator = (assignment operator)
            \param variable The variable to equal
            \return ModuleVariable& self reference
        */
	ModuleVariable& operator= (const ModuleVariable &variable);	
	
        /**
            Look for equality for two variables.
            \param variable the variable to compare to
            \return bool TRUE if equal (or equivalent)
        */
	bool operator== (const ModuleVariable &variable) const;
	
        /**
            Save variable content to XML
            \param document The Main XML document
            \param parentElement The parent element, usually is a XML structure for the ModuleConfiguration
        */
	void saveXML(QDomDocument &document, QDomElement &parentElement);

        /**
            Load XML from a document
            \param element the element containing the XML information
        */
	bool loadXML(QDomElement &element);
	
        /**
            Write a type to string value \ref VARIABLE_TYPE
            i.e. : "double", "float", "sint32", "uint32", "sint16", "uint16","sint8", "uint8", "invalid"
            \param type enum value
            \return QString the string value corresponding
        */
	static QString typeToString(VARIABLE_TYPE type);

        /**
            Convert the string type into the enum value. Please see \ref typeToString.
            \param typeString the string
            \return VARIABLE_TYPE the type (enum value)
        */
	static VARIABLE_TYPE stringToType(const QString &typeString);

public slots:	
	
	//SETS
        /**
            \param value The QVariant containing the value
            \param userUpdate TRUE if the value has been changed from the user, default = false
        */
        void setValue(QVariant value, bool userUpdate = false, QDateTime updateTime = QDateTime::currentDateTime());

        /**
            \param value (float value)
            \param userUpdate TRUE if the value has been changed from the user, default = false
        */
        void setValue(float value, bool userUpdate = false, QDateTime updateTime = QDateTime::currentDateTime());

        /**
            \param value (double value)
            \param userUpdate TRUE if the value has been changed from the user, default = false
        */
        void setValue(double value, bool userUpdate = false, QDateTime updateTime = QDateTime::currentDateTime());

        /**
            \param value (sint32 value)
            \param userUpdate TRUE if the value has been changed from the user, default = false
        */
    void setValue (int value, bool userUpdate = false, QDateTime updateTime = QDateTime::currentDateTime());

        /**
            \param value (uint32 value)
            \param userUpdate TRUE if the value has been changed from the user, default = false
        */
    void setValue (unsigned int value, bool userUpdate = false, QDateTime updateTime = QDateTime::currentDateTime());

        /**
            \param value (sint16 value)
            \param userUpdate TRUE if the value has been changed from the user, default = false
        */
    void setValue (short value, bool userUpdate = false, QDateTime updateTime = QDateTime::currentDateTime());

        /**
            \param value (uint16 value)
            \param userUpdate TRUE if the value has been changed from the user, default = false
        */
    void setValue (unsigned short value, bool userUpdate = false, QDateTime updateTime = QDateTime::currentDateTime());

        /**
            \param value (sint8 value)
            \param userUpdate TRUE if the value has been changed from the user, default = false
        */
        void setValue (char value, bool userUpdate = false, QDateTime updateTime = QDateTime::currentDateTime());

        /**
            \param unsigned char (uint8 value)
            \param userUpdate TRUE if the value has been changed from the user, default = false
        */
        void setValue (unsigned char value, bool userUpdate = false, QDateTime updateTime = QDateTime::currentDateTime());

        /**
            \param value The string containing the formatted value
            \param userUpdate TRUE if the value has been changed from the user, default = false
        */
    void setValue (const QString &value, bool userUpdate = false, QDateTime updateTime = QDateTime::currentDateTime());



        /**
            \param data the string data (formatted value)
            \param size the string size
            \param userUpdate TRUE if the value has been changed from the user, default = false
        */
    void setValue(const unsigned char* data, int size, bool userUpdate = false, QDateTime updateTime = QDateTime::currentDateTime());

        /**
            \param
            \param userUpdate TRUE if the value has been changed from the user, default = false
        */
        void setDeviceID(int id);

        /**
            \param id the interface id
        */
        void setInterfaceID(int id);

        /**
            \return int the interface id
        */
        int getInterfaceID() const;


        /**
            The only one permitted from a script
            Will set the value with a QVariant
            \param value the new value
        */
        Q_INVOKABLE void setUserValue(QVariant value);

        void setUserValue(double value);


        /**
            Set the user value with int type. It is equivalent to setValue(int,true).
            \param value the int value
        */
	void setUserValue(int value);

        /**
            Set the user value with a bool type
            Bool type is not supported by default, It will be converted to the adequate internal type.
            \param value TRUE or FALSE
        */
        void setUserValue(bool value);

        /**
            Activate the variable for refresh
            \param activated TRUE for refresh activated
        */
        Q_INVOKABLE void setActivated(bool activated);
	
public:
	//GETS
        //Q_INVOKABLE will make this getValue invokable by QtScript

        /**
            \return the variable's value
        */
        Q_INVOKABLE QVariant getValue() const;

        /**
            \return the variable's type
        */
        Q_INVOKABLE VARIABLE_TYPE getType() const;

        /**
            \return the variable's memory type
        */
        Q_INVOKABLE VARIABLE_MEMORY_TYPE getMemType() const;

        /**
            \return the variable's name
        */
        Q_INVOKABLE QString getName() const;

        /**
            \return the variable's offset
        */
        Q_INVOKABLE int getOffset() const;

        Q_INVOKABLE unsigned int getSize() const;

        /**
            \return the variable's description
        */
        Q_INVOKABLE QString getDescription() const;

        /**
            \return the variable's deviceID
        */
        Q_INVOKABLE int getDeviceID() const;

        /**
            Get the binary size of the value
            \return int the lenght of the data value
        */
        Q_INVOKABLE int getLength() const;

        /**
            \return bool activated (refresh) state
        */
        Q_INVOKABLE bool getActivated() const;

	
        Q_INVOKABLE QDateTime getUpdateTime() const;

        /**
            Conversion to binary format (little endian) according to \ref VARIABLE_TYPE
            \return QByteArray the binary format
        */
	QByteArray toByteArray() const;




signals:
	
        /**
            Signal sent whent the variable changed
            \param variable (will be == this)
        */
	void valueChanged(ModuleVariable *variable);

        /**
            Signal sent when the user changed the variable (mostly with the GUI).
            This is different from the valueChanged signal that comes from a refresh of the variable from the bus.
            \param variable (will be == this)
        */
	void userChanged(ModuleVariable* variable);

        /**
            Signal sent when the variable will be destroyed
            \param variable (will be == this)
        */
	void aboutToDestroy(ModuleVariable* variable);

    /**
        Signal sent when the variable changes its activation state
    */
    void variableActivated(bool state, ModuleVariable* var=NULL);
	
protected:
	
        ///Variable type (int, float, etc.)
	VARIABLE_TYPE m_type;

        ///Variable memory type (RAM, EEPROM)
	VARIABLE_MEMORY_TYPE m_memType;

        ///Variable name
	QString m_name;

        ///Variable memory offset (in the controller)
        int m_offset;

        ///Variable description
	QString m_description;

        ///Vairiable value
        QVariant m_value;


        ///To make sure the variable knows to which device it belongs
	int m_deviceID;

        ///Interface number
        int m_interfaceID;

        ///True if the variable is activated (will be updated by the scheduler)
        bool m_activated;

        ///Update time
        QDateTime m_updateTime;

private:
        /**
            Constructor (with default arg)
            This constructor is private and cannot be called. This will avoid variables that are undefined or incomplete.
            \param parent The parent object, default = NULL
        */
	ModuleVariable(QObject *parent=NULL);
	
};



#endif

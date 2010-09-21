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

#include "ModuleConfiguration.h"
#include <QFile>
#include <QMessageBox>
#include <QCoreApplication>

ModuleConfiguration::ModuleConfiguration(QObject *parent)
: QObject(parent), m_projectID(-1), m_codeVersion(0), m_processorID(0), m_moduleState(0), m_tableVersion(0), m_deviceID(0)
{
	
}

ModuleConfiguration::ModuleConfiguration(int projectID, int codeVersion, int processorID, int moduleState, int tableVersion, int deviceID, QObject *parent)
 : QObject(parent),
   m_projectID(projectID),
   m_codeVersion(codeVersion),
   m_processorID(processorID),
   m_moduleState(moduleState),
   m_tableVersion(tableVersion),
   m_deviceID(deviceID)
{
	//Starting by searching project file
	QStringList configList = scanConfigurations(QCoreApplication::applicationDirPath() + "/../config",projectID,-1);

	qDebug("Found %i matching configuration (s)",configList.size());
	
	if (configList.size() == 1)
	{
		//Loading configuration, variables only
		loadConfiguration(configList[0], true);
	}
	else if (configList.size() > 1)
	{
		qDebug("More than one configuration found. Load it manually.");
	}
	
}

ModuleConfiguration::ModuleConfiguration(const ModuleConfiguration &cpy)
: QObject(NULL)
{
	m_projectID = cpy.m_projectID;
	m_codeVersion = cpy.m_codeVersion;
	m_processorID = cpy.m_processorID;
	m_moduleState = cpy.m_moduleState;
	m_tableVersion = cpy.m_tableVersion;
	m_deviceID = cpy.m_deviceID;
	
	//Deep Copy variables
	for (int i = 0; i  < cpy.m_variables.size(); i++)
	{
		addVariable(new ModuleVariable(*(cpy.m_variables[i])));
	}
}

ModuleConfiguration::~ModuleConfiguration()
{
	qDebug("ModuleConfiguration::~ModuleConfiguration()");

	//Destroy variables	
	for (int i = 0; i < m_variables.size(); i++)
	{
		delete m_variables[i];
	}
	
	
	qDebug("ModuleConfiguration::~ModuleConfiguration() (done)");
}

ModuleConfiguration& ModuleConfiguration::operator= (const ModuleConfiguration& cpy)
{
	m_projectID = cpy.m_projectID;
	m_codeVersion = cpy.m_codeVersion;
	m_processorID = cpy.m_processorID;
	m_moduleState = cpy.m_moduleState;
	m_tableVersion = cpy.m_tableVersion;
	m_deviceID = cpy.m_deviceID;
	
    //Deep Copy variables
	for (int i = 0; i  < cpy.m_variables.size(); i++)
	{
		addVariable(new ModuleVariable(*(cpy.m_variables[i])));
	}       
	
	return *this;
}

void ModuleConfiguration::prettyPrint(QIODevice &device)
{
	
	QTextStream stream(&device);
	
	stream << "<b>projectID    : </b>" << m_projectID << "\r\n";
	stream << "<b>codeVersion  : </b>" << m_codeVersion << "\r\n";
	stream << "<b>processorID  : </b>" << m_processorID << "\r\n";
	stream << "<b>moduleState  : </b>" << m_moduleState << "\r\n";
	stream << "<b>tableVersion : </b>" << m_tableVersion << "\r\n";
	stream << "<b>deviceID     : </b>" << m_deviceID << "\r\n";
}


void ModuleConfiguration::saveConfiguration(const QString &filename)
{
        QDomDocument document("INTROLAB-NETWORKVIEWER");
	
	QDomElement element = document.createElement("ModuleConfiguration");
	
	element.setAttribute("projectID",QString::number(m_projectID));
	element.setAttribute("codeVersion",QString::number(m_codeVersion));
	element.setAttribute("processorID",QString::number(m_processorID));
	element.setAttribute("moduleState",QString::number(m_moduleState));
	element.setAttribute("tableVersion",QString::number(m_tableVersion));
	element.setAttribute("deviceID",QString::number(m_deviceID));
	
	for (int i = 0; i<m_variables.size(); i++)
	{
		m_variables[i]->saveXML(document,element);
	}
	
	document.appendChild(element);
	
	QFile file(filename);
	file.open(QIODevice::WriteOnly);
	QTextStream stream(&file);
	
	
	document.save(stream, 5);
}

int ModuleConfiguration::size()
{
	return m_variables.size();
}

ModuleVariable* ModuleConfiguration::operator[] (int index)
{
	Q_ASSERT(index < m_variables.size() && index >= 0);
	return m_variables[index];
}

void ModuleConfiguration::addVariable(ModuleVariable *variable)
{
	m_variables.push_back(variable);

	//Make sure the variable knows to which device ID it belongs
	variable->setDeviceID(m_deviceID);

	//Connect signals for user modification
	connect(variable, SIGNAL(userChanged(ModuleVariable*)), this, SLOT(variableUpdated(ModuleVariable*)));

    //Emit variable added
    emit variableAdded(variable);
}


int ModuleConfiguration::getProjectID()
{
    return m_projectID;
}

int ModuleConfiguration::getCodeVersion()
{
    return m_codeVersion;
}

int ModuleConfiguration::getProcessorID()
{
    return m_processorID;
}

int ModuleConfiguration::getModuleState()
{
    return m_moduleState;
}

int ModuleConfiguration::getTableVersion()
{
    return m_tableVersion;
}

int ModuleConfiguration::getDeviceID()
{
    return m_deviceID;
}

void ModuleConfiguration::setDeviceID(int id)
{
	m_deviceID = id;
}

bool ModuleConfiguration::loadConfiguration(const QString &filename, bool variablesOnly)
{
        QDomDocument doc("INTROLAB-NETWORKVIEWER");

	//Clean up memory
	for (int i = 0; i < m_variables.size(); i++)
	{
		delete m_variables[i];
	}
	
	m_variables.clear();

	QFile file(filename);

	//Open the file
	if(!file.open(QIODevice::ReadOnly))
	{
		QMessageBox::warning(0, "Warning", QString("Unable to open file : ") + filename, QMessageBox::Ok);
		return false;
	}

	qDebug() << "Loading : "<<filename;

	//Set XML content
	if (!doc.setContent(&file)) {
		file.close();
		QMessageBox::warning(0, "Warning", QString("Unable to read XML content from file : ") + filename, QMessageBox::Ok);
		return false;
	}

	//Verify if we have a configuration
	//Get the root element
	QDomElement e = doc.documentElement(); // <ModuleConfiguration> element
	if(e.tagName() != "ModuleConfiguration")
	{
		qDebug() <<"No ModuleConfiguration found. Element is "<< e.tagName();
		return false;
	}

	if (!variablesOnly)
	{
		//Get parameters	
		m_projectID = e.attribute("projectID").toInt();
		m_codeVersion = e.attribute("codeVersion").toInt();
		m_processorID = e.attribute("processorID").toInt();
		m_moduleState = e.attribute("moduleState").toInt();
		m_tableVersion = e.attribute("tableVersion").toInt();
		m_deviceID = e.attribute("deviceID").toInt();
	}
	//qDebug() << "Found element : "<<e.tagName();

	//Iterate through all child (this would be variables)
	QDomNode n = e.firstChild();

	while(!n.isNull())
	{
		e = n.toElement(); // try to convert the node to an element.


		if(!e.isNull())
		{
			if(e.tagName() == "ModuleVariable")
			{
				//Creating variable from XML
				ModuleVariable *var = new ModuleVariable(e);

				//Adding the variable
				addVariable(var);

			}
		}//if e.isNull
		else
		{
			qDebug("NULL element");
		}

		//Next item
		n = n.nextSibling();
	}//while n.isNull

	return true;
}


void ModuleConfiguration::variableUpdated(ModuleVariable *var)
{
	//qDebug("ModuleConfiguration::variableUpdated(const ModuleVariable &var)");
	emit variableWrite(var);
}

QStringList ModuleConfiguration::scanConfigurations(const QString &basePath, int project_id, int device_id)
{
	qDebug("ModuleConfiguration::scanConfigurations");

	QStringList scanList;
	
	//Plugins directory
	QDir dir(basePath);

	if (!dir.exists())
	{
		qWarning("ModuleConfiguration::scanConfigurations : plugins directory not found");
		qWarning() << "ModuleConfiguration::scanConfigurations : Current path : " << dir.absolutePath();
	}
	else
	{
		qDebug() << "ModuleConfiguration::scanConfigurations : Scanning : " << dir.absolutePath();
		recursiveScan(scanList, project_id, device_id, dir);
	}
	
	return scanList;
}

void ModuleConfiguration::recursiveScan(QStringList &configList, int project_id, int device_id, QDir directory, int level)
{
	qDebug() << "ModuleConfiguration::recursiveScan :  Path : " << directory.path() << " project_id : "<<project_id<<" device_id : "<<device_id;

	if (level < 10 && directory.exists())
	{

		QFileInfoList myInfoList = directory.entryInfoList();

		for (int i = 0; i < myInfoList.size(); i++)
		{

			if (!myInfoList[i].fileName().startsWith("."))
			{
				if (myInfoList[i].isDir())
				{
					recursiveScan(configList, project_id, device_id, QDir(directory.path() + QDir::separator() + myInfoList[i].fileName()), level + 1);
				}
				else
				{
					//Look for standard configuration file xml type
					if (myInfoList[i].fileName().contains(".xml"))
					{
						ModuleConfiguration tempConfig;
						tempConfig.loadConfiguration(directory.path() + QDir::separator() + myInfoList[i].fileName());						
					
						//If device_id does not matter (-1) put to actual device id
						int my_device_id = device_id;
						if (device_id == -1)
						{
							my_device_id = tempConfig.getDeviceID();	
						}
						
						//Insert configuration if it fits the project_id and device_id
						if (tempConfig.getProjectID() == project_id && tempConfig.getDeviceID() == my_device_id)
						{
							qDebug() << "Found a valid configuration, inserting : " << directory.path() + QDir::separator() + myInfoList[i].fileName();
							configList.push_back(directory.path() + QDir::separator() + myInfoList[i].fileName());
						}
					}
				}
			}//does not start with "."
		}//for infoList
	}
	else
	{
		qWarning("ModuleConfiguration::recursiveScan : error level : %i",level);
	}

}

ModuleVariable* ModuleConfiguration::getVariableNamed(QString name)
{
	 ModuleVariable *variable = NULL;
	 
	 for (int i = 0; i < m_variables.size(); i++)
	 {
		if (m_variables[i]->getName() == name)
		{
			return m_variables[i];
		}
	 }
	 
	 return variable;
}

int ModuleConfiguration::indexOf(ModuleVariable *var)
{
    return m_variables.lastIndexOf(var);
}

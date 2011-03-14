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

#ifndef _MODULE_CONFIGURATION_H_
#define _MODULE_CONFIGURATION_H_

#include "ModuleVariable.h"
#include <QDir>
#include <QStringList>


/**
  A ModuleConfiguration holds all the information of the module and contains a list of \ref ModuleVariable.
  \author Dominic Letourneau
  \date Dec 2009
*/
class ModuleConfiguration : public QObject 
{

	Q_OBJECT;
	
public:
	

        /**
            Constructor
            \param parent The ModuleConfiguration parent, defaults to NULL
        */
        ModuleConfiguration(QObject *parent=NULL);


        /**
            Copy constructor
            \param cpy the ModuleConfiguration to copy. We will perform a deep copy on variables.
        */
	ModuleConfiguration(const ModuleConfiguration &cpy);

        /**
            Constructor
            \param projectID, The module project ID
            \param codeVersion, The module code version
            \param moduleState the module state defined from \ref DeviceFactory
            \param tableVersion the protocol version
            \param deviceID the address on the bus
            \param parent The parent QObject, defaults to NULL
        */
	ModuleConfiguration(int projectID, int codeVersion, int processorID, int moduleState, int tableVersion, int deviceID, QObject *parent=NULL);
	
        /**
            Will destroy all internal variables.
        */
	virtual ~ModuleConfiguration();

        /**
            \param cpy The ModuleConfiguration to copy
        */
	ModuleConfiguration& operator= (const ModuleConfiguration &cpy);
	
        /**
            Print the configuration values into a stream. Will use rich text format.
            \param device que QIODevice to print to
        */
        void prettyPrint(QIODevice &device);

        /**
            Save the configuration in XML format
            \param filename the full path where to save
        */
        void saveConfiguration(const QString &filename);

        /**
            Load the configuration from XML format
            \param filename the full path
            \param variablesOnly Defaults to false. When true, we will keep internal module configuration and load only variable descriptions.
            This is useful because XML configurations are created with undefined fields and must not overwrite detected values on the bus.
        */
	bool loadConfiguration(const QString &filename, bool variablesOnly = false);

        /**
            \return int The count of variables
        */
	int size();

        /**
            Enable easy access to variables with the [] operator.
            \param index the variable index. User must make sure the index in correct, otherwise the assertion will fail.
            \return ModuleVariable* The associated variable
        */
	ModuleVariable* operator[] (int index);

        /**
            Get a variable with its name.
            \param name The variable's name
            \return ModuleVariable* the variable. Returns NULL if the variable was not found.
        */
	ModuleVariable* getVariableNamed(QString name);


        /**
            Get the index of a variable
            \param var The variable
            \return int the index of it, -1 if not found
        */
        int indexOf(ModuleVariable *var);

        /**
            Add a variable to the ModuleConfiguration
            \param variable The variable to add. The module configuration will then own the variable and will be responsible to delete it.
        */
	void addVariable(ModuleVariable *variable);
	
        //GETS

        /**
            \return m_projectID
        */
	int getProjectID();

        /**
            \return m_codeVersion
        */
	int getCodeVersion();

        /**
            \return m_processorID
        */
	int getProcessorID();

        /**
            \return m_moduleState
        */
	int getModuleState();

        /**
            \return m_tableVersion
        */
	int getTableVersion();

        /**
            \return m_deviceID
        */
	int getDeviceID();
	
	//SETS

        /**
            Set m_deviceID
            \param id the device ID
        */
	void setDeviceID(int id);

	/**
         * Will scan xml configuration files automatically and traverse all sub-directory. Possible configurations will be appended in the QStringList result.
	 * \param basePath The path where to start the scan.
	 * \param project_id look for the right project id
         * \param device_id Look for a particular device_id -1 = any module
         * \return QStringList the full path of the XML configuration files matching the project_id and device_id
	 */
	static QStringList scanConfigurations(const QString &basePath, int project_id, int device_id = -1);

signals:

    /**
        This signal is emitted when a variable has been changed from the user. The variable must then be
        written to the \ref CANDevice.
    */
	void variableWrite(ModuleVariable*  variable);


    /**
        Emit the signal when a new variable is added
    */
    void variableAdded(ModuleVariable* variable);

    /**
        Emit the signal when a new variable is added
    */
    void variableRemoved(ModuleVariable* variable);


protected slots:

        /**
            Will receive signal from variables with they are updated. If this happens, will also emit the variableWrite signal
            so we can write it to the bus.
        */
	void variableUpdated(ModuleVariable *var);


protected:
	
	/**
         * Internal function to traverse directory for XML configuration scans.
         * \param configList The list of configuration, will append configuration if found
         * \param project_id Look for the specified project_id
         * \param device_id Look for the specified device_id
	 * \param directory The current QDir
	 * \param level the recursion level, 0 = basePath
	 */
	static void recursiveScan(QStringList &configList, int project_id, int device_id, QDir directory, int level = 0);
	
        ///The project version used to identify which configuration to load
	int m_projectID;

        ///The code version of the project
	int m_codeVersion;

        ///The processor ID (each microcontroller type has a different id)
	int m_processorID;

        ///The module state
	int m_moduleState;

        ///The internal module table version (protocol version)
	int m_tableVersion;

        ///The device_id (address) on the bus
	int m_deviceID;
	
	///List of all variables
	QList<ModuleVariable*> m_variables;
};

#endif


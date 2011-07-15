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


#ifndef _NETWORK_MODULE_H_
#define _NETWORK_MODULE_H_

#include "ModuleConfiguration.h"
#include <QTime>

/**
  The Network module will contain the module configuration and variables.
  \author Dominic Letourneau
*/
class NetworkModule : public QObject
{
	Q_OBJECT;
	
public:

        /**
          Constructor
          \param config The default configuration (can be empty)
          \parent The parent object, default = NULL
        */
	NetworkModule(const ModuleConfiguration &config, QObject *parent = NULL);


        /**
            Destructor
        */
	virtual ~NetworkModule();

        /**
            Utility function to get a variable from the ModuleConfiguration with its name
            \param name The name of the variable
            \return ModuleVariable* the variable with name, NULL if not found
        */
        ModuleVariable* getVariable(const QString &name);

        /**
            \return int the count of variables
        */
        int getNumVariable();


        /**
            Create a custom variable (mostly for scripts)
            \param name the name of the variable
            \return ModuleVariable *the newly created module variable
        */
        bool createScriptVariable(const QString &_name, const QString &_description = "");

        /**
            Utility function to get the variables inside the ModuleConfiguration data structure
            \param index the index of the variable
            \return ModuleVariable* the variable, NULL if out of range
        */
        ModuleVariable* getVariable(int index);

        /**
            Get current configurationChanged();
            \return the ModuleConfiguration*
        */
	ModuleConfiguration* getConfiguration();

        /**
            \return bool The active flag
        */
        bool active();

        /**
            Change the active flag.
            \param active TRUE when the module is activated, FALSE otherwise
        */
        void setActive(bool active);

        /**
            Set the last time the module have been seen on the bus
            \param time the current time
        */
        void setUpdateTime(const QTime &time = QTime::currentTime());

        /**
            Get the last time the module have been seen on the bus
        */
        QTime getUpdateTime() const;

signals:

        /**
            We emit this signal when a variable has been written by the user
            \param variable The modified variable
        */
	void variableWrite(ModuleVariable *variable);

        /**
            We emit this signal when the configuration changed
        */
        void configurationChanged();

        /**
            We emit this signal when we are destroyed
         */
	void moduleDestroyed();

protected slots:

        /**
            Called when a variable has been updated by the user. This will cause the variableWrite signal to be emitted.
            \param variable The variable taht has been updated
        */
	void variableUpdate(ModuleVariable *variable);

protected:
	
        ///Module configuration \ref ModuleConfiguration
	ModuleConfiguration m_configuration;

        ///TRUE if the module is active on the bus
        bool m_active;

        ///Last time the module have been seen on the bus
        QTime m_lastUpdateTime;
};


#endif


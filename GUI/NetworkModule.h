/**
     Copyright (C) 2009-2010 IntRoLab
     http://introlab.gel.usherbrooke.ca
     Dominic Letourneau, ing. M.Sc.A.
     Dominic.Letourneau@USherbrooke.ca
 */


#ifndef _NETWORK_MODULE_H_
#define _NETWORK_MODULE_H_

#include "ModuleConfiguration.h"


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
	
};


#endif


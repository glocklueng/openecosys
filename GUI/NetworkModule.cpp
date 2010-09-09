/**
     Copyright (C) 2009-2010 IntRoLab
     http://introlab.gel.usherbrooke.ca
     Dominic Letourneau, ing. M.Sc.A.
     Dominic.Letourneau@USherbrooke.ca
 */

#include "NetworkModule.h"

NetworkModule::NetworkModule(const ModuleConfiguration &config, QObject *parent)
: QObject(parent), m_configuration(config), m_active(true)
{
    setProperty("name",QString("NetworkModule:") + QString::number(m_configuration.getDeviceID()));
    connect(&m_configuration, SIGNAL(variableWrite(ModuleVariable*)),this,SLOT(variableUpdate(ModuleVariable*)));
}

NetworkModule::~NetworkModule()
{
	emit moduleDestroyed();
	qDebug("NetworkModule::~NetworkModule()");
}

int NetworkModule::getNumVariable()
{
    return m_configuration.size();
}

ModuleVariable*  NetworkModule::getVariable(const QString &name)
{
	for (int i = 0; i<m_configuration.size(); i++)
	{
		if (m_configuration[i]->getName() == name)
		{
			return m_configuration[i];
		}
	}
	
	return NULL;
}

ModuleVariable* NetworkModule::getVariable(int index)
{
	if (index < m_configuration.size())
	{
		return m_configuration[index];
	}
	
	return NULL;
}

ModuleConfiguration* NetworkModule::getConfiguration()
{
	return &m_configuration;
}

void NetworkModule::variableUpdate(ModuleVariable *variable)
{
	//qDebug("NetworkModule::variableUpdate(ModuleVariable *variable)");
	emit variableWrite(variable);
}

bool NetworkModule::active()
{
    return m_active;
}

void NetworkModule::setActive(bool active)
{
    m_active = active;
}

bool NetworkModule::createScriptVariable(const QString &_name, const QString &_description )
{
    ModuleVariable *var = m_configuration.getVariableNamed(_name);

    if (var == NULL)
    {
        ModuleVariable *var = new ModuleVariable(ModuleVariable::DOUBLE,_name,ModuleVariable::SCRIPT_VARIABLE,-1,_description);
        int index = m_configuration.size();
        m_configuration.addVariable(var);
        return true;
    }

    return false;

}

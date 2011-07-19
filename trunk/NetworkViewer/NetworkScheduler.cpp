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


#include "NetworkScheduler.h"
#include "NETVInterfaceManager.h"
#include <QTime>

NetworkScheduler::NetworkScheduler(NETVInterfaceManager *manager)
    : QObject(manager),
    m_manager(manager),
    m_schedulerTimer(NULL),
    m_aliveTimer(NULL)
{
    Q_ASSERT(m_manager != NULL);


    m_aliveTimer = new QTimer(this);
    connect(m_aliveTimer,SIGNAL(timeout()), this, SLOT(schedulerAliveRequest()));
    m_aliveTimer->start(1000); //1sec alive requests.


    m_schedulerTimer = new QTimer(this);
    connect(m_schedulerTimer,SIGNAL(timeout()),this,SLOT(schedulerUpdate()));
    m_schedulerTimer->start(10);//10ms timer
}

void NetworkScheduler::schedulerUpdate()
{

    //qDebug("NetworkScheduler::schedulerUpdate() nbVar : %i",m_variableScheduleList.size());

    //How many variables should we update at once?
    for (int iter = 0; iter < 1; iter++)
    {
        //verify that we have something to schedule
        if (m_variableScheduleList.size())
        {
            //Get first variable to schedule
            ModuleVariable *var = m_variableScheduleList.front();
            m_variableScheduleList.pop_front();

            if (var->getActivated())
            {
                //Request variable on the bus
                m_manager->requestVariable(var);

                //At the end for rescheduling if activated...
                m_variableScheduleList.push_back(var);
            }
        }
        else
        {
            break;  
        }

    } //iter
}

void NetworkScheduler::addModule(NetworkModule* module)
{
    //qDebug("NetworkScheduler::addModule %p, conf size: %i",module,module->getConfiguration()->size());

    if (!m_modules.contains(module))
    {
        m_modules.push_back(module);

        //Add variables to schedule list
        ModuleConfiguration *conf = module->getConfiguration();

        for (int i = 0; i < conf->size(); i++)
        {
            //Make sure variable is scheduled only once
            addScheduledVariable((*conf)[i]);
        }
    }

    //If new variables are available, shedule them
    connect(module->getConfiguration(),SIGNAL(variableAdded(ModuleVariable*)),this,SLOT(addScheduledVariable(ModuleVariable*)));

    //If variables are removed from module, remove from schedule
    connect(module->getConfiguration(),SIGNAL(variableRemoved(ModuleVariable*)),this,SLOT(removeScheduledVariable(ModuleVariable*)));

}

void NetworkScheduler::removeModule(NetworkModule* module)
{
    //qDebug("NetworkScheduler::removeModule(NetworkModule* module = %p",module);

    //Remove variables for scheduling
    ModuleConfiguration *conf = module->getConfiguration();

    //Disconnect new variables are available, shedule them
    disconnect(module->getConfiguration(),SIGNAL(variableAdded(ModuleVariable*)),this,SLOT(addScheduledVariable(ModuleVariable*)));

    //Disconnect variables are removed from module, remove from schedule
    disconnect(module->getConfiguration(),SIGNAL(variableRemoved(ModuleVariable*)),this,SLOT(removeScheduledVariable(ModuleVariable*)));

    for (int i = 0; i < conf->size(); i++)
    {
        removeScheduledVariable((*conf)[i]);
    }

    m_modules.removeAll(module);
}

void NetworkScheduler::schedulerAliveRequest()
{
    if (m_manager)
    {
        m_manager->sendAliveRequest();
    }
}

void NetworkScheduler::addScheduledVariable(ModuleVariable *var)
{
    if (!m_variableScheduleList.contains(var) && var->getMemType() < ModuleVariable::SCRIPT_VARIABLE)
    {
        qDebug() << "Adding (NEW) variable for scheduling:"<<var->getName()<<" device id: "<<var->getDeviceID();

        //Connect variable activation change
        connect(var,SIGNAL(variableActivated(bool,ModuleVariable*)),this,SLOT(variableActivated(bool,ModuleVariable*)));

        //Connect variable for auto removal when destroyed
        connect(var,SIGNAL(aboutToDestroy(ModuleVariable*)),this,SLOT(removeScheduledVariable(ModuleVariable*)));

        m_variableScheduleList.push_back(var);
    }
}


void NetworkScheduler::removeScheduledVariable(ModuleVariable *var)
{
    if (m_variableScheduleList.contains(var))
    {
        qDebug() << "Removing (OLD) variable for scheduling:"<<var->getName()<<" device id: "<<var->getDeviceID();

        //Disconnect variable activation change
        disconnect(var,SIGNAL(variableActivated(bool,ModuleVariable*)),this,SLOT(variableActivated(bool,ModuleVariable*)));

        //Disconnect variable for auto removal when destroyed
        disconnect(var,SIGNAL(aboutToDestroy(ModuleVariable*)),this,SLOT(removeScheduledVariable(ModuleVariable*)));

        m_variableScheduleList.removeAll(var);
    }
}


void NetworkScheduler::variableActivated(bool activated, ModuleVariable *var)
{
    if (activated)
    {
        addScheduledVariable(var);
    }
    else
    {
        removeScheduledVariable(var);
    }

}

int NetworkScheduler::getAliveRequestInterval()
{
    Q_ASSERT(m_aliveTimer);
    return m_aliveTimer->interval();
}

int NetworkScheduler::getVariableRequestInterval()
{
    Q_ASSERT(m_schedulerTimer);
    return m_schedulerTimer->interval();
}


void NetworkScheduler::setAliveRequestInterval(int value)
{
    if (value > 0)
    {
        qDebug("void NetworkScheduler::setAliveRequestInterval(int value = %i)",value);
        m_aliveTimer->setInterval(value);
    }
    else
    {
        qDebug("NetworkScheduler::setAliveRequestInterval invalid value : %i",value);
    }
}


void NetworkScheduler::setVariableRequestInterval(int value)
{
    if (value > 0)
    {
        qDebug("NetworkScheduler::setVariableRequestInterval(int value = %i)",value);
        m_schedulerTimer->setInterval(value);
    }
    else
    {
        qDebug("NetworkScheduler::setVariableRequestInterval invalid value : %i",value);
    }
}


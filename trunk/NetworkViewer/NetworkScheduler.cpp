/**
     Copyright (C) 2009-2010 IntRoLab
     http://introlab.gel.usherbrooke.ca
     Dominic Letourneau, ing. M.Sc.A.
     Dominic.Letourneau@USherbrooke.ca
 */


#include "NetworkScheduler.h"
#include "NetworkView.h"
#include <QTime>

NetworkScheduler::NetworkScheduler(NetworkView *view)
    : QObject(view),
    m_view(view),
    m_schedulerTimer(NULL),
    m_aliveTimer(NULL)
{
    Q_ASSERT(m_view != NULL);


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
                //qDebug() << "Schedululing : "<<var->getName()<<" device id : "<<var->getDeviceID();
                m_view->requestVariable(var);


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
    qDebug("NetworkScheduler::addModule %p, conf size: %i",module,module->getConfiguration()->size());

    if (!m_modules.contains(module))
    {
        m_modules.push_back(module);

        //Add variables to schedule list
        ModuleConfiguration *conf = module->getConfiguration();

        for (int i = 0; i < conf->size(); i++)
        {
            //Make sure variable is scheduled only once
            if (!m_variableScheduleList.contains((*conf)[i]))
            {

                if ((*conf)[i]->getMemType() < ModuleVariable::SCRIPT_VARIABLE)
                {
                    qDebug() << "Adding to scheduler " << (*conf)[i]->getName() << " device id: "<<(*conf)[i]->getDeviceID();
                    m_variableScheduleList.push_back((*conf)[i]);

                    //Connect variable activation change
                    connect((*conf)[i],SIGNAL(variableActivated(bool,ModuleVariable*)),this,SLOT(variableActivated(bool,ModuleVariable*)));
                }
            }
        }


    }

    //If new variables are available, shedule them
    connect(module->getConfiguration(),SIGNAL(variableAdded(ModuleVariable*)),this,SLOT(addScheduledVariable(ModuleVariable*)));
}

void NetworkScheduler::removeModule(NetworkModule* module)
{
    qDebug("NetworkScheduler::removeModule(NetworkModule* module = %p",module);

    //Remove variables for scheduling
    ModuleConfiguration *conf = module->getConfiguration();

    for (int i = 0; i < conf->size(); i++)
    {          
        m_variableScheduleList.removeAll((*conf)[i]); 
    }

    m_modules.removeAll(module);
}

void NetworkScheduler::schedulerAliveRequest()
{
    if (m_view)
    {
        m_view->sendAliveRequest();
    }
}

void NetworkScheduler::addScheduledVariable(ModuleVariable *var)
{

    if (!m_variableScheduleList.contains(var) && var->getMemType() < ModuleVariable::SCRIPT_VARIABLE)
    {
        qDebug() << "Adding (NEW) variable for scheduling:"<<var->getName()<<" device id: "<<var->getDeviceID();

        //Connect variable activation change
        connect(var,SIGNAL(variableActivated(bool,ModuleVariable*)),this,SLOT(variableActivated(bool,ModuleVariable*)));

        m_variableScheduleList.push_back(var);
    }
}

void NetworkScheduler::variableActivated(bool activated, ModuleVariable *var)
{
    if (activated)
    {
        qDebug() << "Adding variable for scheduling:"<<var->getName()<<" device id: "<<var->getDeviceID();
        if (!m_variableScheduleList.contains(var))
        {
            m_variableScheduleList.push_back(var);
        }
    }
    else
    {
        qDebug() << "Removing variable for scheduling:"<<var->getName()<<" device id: "<<var->getDeviceID();
        m_variableScheduleList.removeAll(var);
    }

}



/**
     Copyright (C) 2009-2010 IntRoLab
     http://introlab.gel.usherbrooke.ca
     Dominic Letourneau, ing. M.Sc.A.
     Dominic.Letourneau@USherbrooke.ca
 */


#ifndef _NETWORKSCHEDULER_H_
#define _NETWORKSCHEDULER_H_

#include <QObject>
#include <QTimer>
#include "NetworkModule.h"

class NetworkView;


/**
    Module responsible of making periodical queries for variables (simple implementation)
    \author Dominic Letourneau
*/
class NetworkScheduler : public QObject
{
    Q_OBJECT;

public:

    /**
        Constructor
        \param view The parent view
    */
    NetworkScheduler(NetworkView *view);

    /**
        Add a module for scheduling
        \param module The module to add
    */
    void addModule(NetworkModule* module);


    /**
        Remove a module for scheduling
        \param module The module to remove
    */
    void removeModule(NetworkModule* module);

protected slots:

    /**
        Add a variable to the schedule
        \param var the Scheduled variable
    */
    void addScheduledVariable(ModuleVariable *var);

    /**
        Called by the timer m_schedulerTimer (periodic)
        Will schedule variables to be updated
    */
    void schedulerUpdate();

    /**
        Called by the timer m_aliveTimer (periodic)
        Will shedule alive requests
    */
    void schedulerAliveRequest();

    /**
        Called when a variable changes its activation state
        \param state the state of activation
        \param var the variable pointer
    */
    void variableActivated(bool activated, ModuleVariable *var);

protected:
    ///The parent view
    NetworkView *m_view;
    ///The variable scheduler timer
    QTimer *m_schedulerTimer;
    ///The list of scheduled modules
    QList<NetworkModule*> m_modules;
    ///The alive timer
    QTimer *m_aliveTimer;
    ///The scheduled variables (round robin)
    QList<ModuleVariable*> m_variableScheduleList;
};


#endif /* NETWORKSCHEDULER_H_ */

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


#ifndef _NETWORKSCHEDULER_H_
#define _NETWORKSCHEDULER_H_

#include <QObject>
#include <QTimer>
#include "NetworkModule.h"

class NETVInterfaceManager;

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
    NetworkScheduler(NETVInterfaceManager *manager);

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

    /**
      \return The alive request interval in ms
    */
    int getAliveRequestInterval();

    /**
        \return The variable request interval in ms
    */
    int getVariableRequestInterval();

public slots:

    /**
        Set the alive request interval
        \param value the interval in ms
    */
    void setAliveRequestInterval(int value);

    /**
        Set the variable request interval
        \param value the interval in ms
    */
    void setVariableRequestInterval(int value);



protected slots:

    /**
        Add a variable to the schedule
        \param var the Scheduled variable
    */
    void addScheduledVariable(ModuleVariable *var);


    /**
        Remove a variable from scheduling
        \param var the variable to remove from schedule
    */
    void removeScheduledVariable(ModuleVariable *var);

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

    ///The parent manager
    NETVInterfaceManager *m_manager;
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

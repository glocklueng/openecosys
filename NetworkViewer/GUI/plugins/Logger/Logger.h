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

#ifndef _LOGGER_H_
#define _LOGGER_H_

#include "BasePlugin.h"
#include "NetworkModule.h"
#include "ModuleVariableTableWidget.h"
#include <QTimer>
#include <QFile>
#include <QDateTime>
#include "ui_Logger.h"


/**
    Event based logger plugin
    \author Dominic Letourneau
*/
class Logger : public BasePlugin
{
	Q_OBJECT;


public:

        /**
            Constructor
            \param view the parent NetworkView
        */
        Logger(NetworkView *view);

        ///init not used right now
	virtual void init();

        ///terminate not used right now
	virtual void terminate();


        void addVariable(ModuleVariable *variable);


        void removeVariable(ModuleVariable *variable);

protected slots:

        /**
            Called when a new variable is added in the ModuleVariableTableWidget
            \param variable The newly added variable
        */
        void variableAdded(ModuleVariable* variable);

        /**
            Called when a variable is removed from the table
            \param variable The removed variable
        */
        void variableRemoved(ModuleVariable* variable);

        /**
            Called when the value of a variable is changed. The variable
            will then be logged if the logger is enabled.
            \param variable The modified variable
        */
        void variableValueChanged(ModuleVariable* variable);

        /**
            User changed the file name manually
            \param the file name
        */
        void lineEditTextChanged(QString value);


        ///Start button clicked
        void startButtonClicked();
        ///Stop button clicked
        void stopButtonClicked();
        ///File button clicked
        void fileButtonClicked();

        /**
            Change the state of the logger
            \param state TRUE if the logger is enabled
        */
        void setLogging(bool state);

protected:

        /**
            Event Handler, will process \ref BasePluginEvent
            \param e The event to handle.
        */
        bool event ( QEvent * e );

        ///UI
        Ui::Logger m_ui;

        ///The table containing the variables
        ModuleVariableTableWidget *m_table;

        ///The variable list
        QList<ModuleVariable*> m_varList;

        ///The log file
        QFile m_file;

        ///Enable/Disable flag
        bool m_logging;

        ///Starting time
        QTime m_startTime;

};

#endif /* _LOGGER_H_ */

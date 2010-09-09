/**
     Copyright (C) 2009-2010 IntRoLab
     http://introlab.gel.usherbrooke.ca
     Dominic Letourneau, ing. M.Sc.A.
     Dominic.Letourneau@USherbrooke.ca
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

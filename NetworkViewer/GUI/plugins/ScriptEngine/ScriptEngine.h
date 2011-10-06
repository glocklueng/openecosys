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

#ifndef ScriptEngine_H_
#define ScriptEngine_H_

#include "BasePlugin.h"
#include "NetworkModule.h"
#include <QScriptEngine>
#include <QScriptEngineAgent>
#include <QTimer>
#include <QTime>
#include <QSlider>
#include "ui_ScriptEngine.h"
#include <QEvent>



/**
  This event is required for adequate termination of the script within
  the script with the stop() function.

  Otherwise, we have a recursive call to runButtonClicked() slot.
*/
class StopScriptEvent : public QEvent
{
public:

    StopScriptEvent()
        : QEvent(QEvent::User)
    {

    }

};





class ModuleVariableSlider : public QSlider
{
    Q_OBJECT;

    public:

    ModuleVariableSlider(QWidget *parent, ModuleVariable *variable = NULL, double min_value = 0, double max_value = 0)
        : QSlider(Qt::Horizontal, parent), m_variable(variable)
    {

        if (m_variable)
        {
            //Set Range of slider
            setMinimum(min_value);
            setMaximum(max_value);

            //Set value of slider
            setValue(m_variable->getValue().toInt());

            //connect signals
            connect(this,SIGNAL(valueChanged(int)),this,SLOT(sliderValueChanged(int)));

            //connect variable signals
            connect(m_variable,SIGNAL(valueChanged(ModuleVariable*)),this,SLOT(variableValueChanged(ModuleVariable*)));
            connect(m_variable,SIGNAL(aboutToDestroy(ModuleVariable*)),this,SLOT(variableAboutToDestroy(ModuleVariable*)));
        }


    }

signals:
    void terminateSlider();

protected slots:

    void variableAboutToDestroy(ModuleVariable* var)
    {
        if (m_variable == var)
        {
            m_variable =  NULL;
            emit terminateSlider();
        }
    }

    void variableValueChanged(ModuleVariable *var)
    {
        if (var)
        {
            if (value() != var->getValue().toInt())
            {
                setValue(var->getValue().toInt());
            }
        }
    }

    void sliderValueChanged(int value)
    {
        if(m_variable)
        {
            m_variable->setUserValue(value);
        }
    }



protected:

    ModuleVariable *m_variable;

};





/**

  Documentation.
 */
class ScriptEngine : public BasePlugin
{
	Q_OBJECT;


public:

	ScriptEngine(NetworkView *view);

	virtual void init();
	virtual void terminate();
        bool scopeRequest(int module_id, int variable_id);
        bool createPseudoModule(int module_id);
        bool addScriptVariable(int module_id, QString name, QString description = "");
        bool addSliderControl(int module_id, int variable_id, double min, double max);
        bool stop();

protected slots:

        void moduleAdded(NetworkModule *module);
        void moduleRemoved(NetworkModule *module);
	void timerUpdate();
        void loadButtonClicked();
        void runButtonClicked();
	void saveButtonClicked();
        void updatePeriod(int period);
        void helpButtonClicked();


protected:

        virtual bool event ( QEvent * e );


        void updateEngineVariables(bool modulesOnly = false);
	QTimer *m_timer;
	Ui::ScriptEngine m_ui;
	QList<NetworkModule*> m_modules;
        QScriptEngine m_scriptEngine;
        QTime m_startTime;
        bool m_running;
        QString m_lastPath;
        int m_cycle;
};

#endif /* ScriptEngine_H_ */

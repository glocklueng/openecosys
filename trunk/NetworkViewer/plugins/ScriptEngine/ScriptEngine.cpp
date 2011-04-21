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

#include "ScriptEngine.h"
#include "NetworkView.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QWebView>
#include <QCoreApplication>

//This will insert the plugin in the dictionary...
static int sample_plugin_init = BasePlugin::registerPlugin("ScriptEngine",new BasePlugin::PluginFactory<ScriptEngine>());


QScriptValue addSliderControlFunction(QScriptContext *context, QScriptEngine  *engine)
{

    QScriptValue calleeData = context->callee().data();
    ScriptEngine *myScriptEngine = qobject_cast<ScriptEngine*>(calleeData.toQObject());

    if (context->argumentCount() == 4)
    {
        int module_id = context->argument(0).toInt32();
        int variable_id = context->argument(1).toInt32();
        double min_value = context->argument(2).toNumber();
        double max_value = context->argument(3).toNumber();

        qDebug() << "Should add slider variable id " << variable_id << " from module : " << module_id << " min: " << min_value << " max: "<<max_value;

        return engine->newVariant(myScriptEngine->addSliderControl(module_id,variable_id,min_value,max_value));
    }
    else
    {
        return engine->newVariant(false);
    }

}

QScriptValue stopFunction(QScriptContext *context, QScriptEngine *engine)
{
    bool retval = false;
    QScriptValue calleeData = context->callee().data();
    ScriptEngine *myScriptEngine = qobject_cast<ScriptEngine*>(calleeData.toQObject());
    if (myScriptEngine)
    {
        retval = myScriptEngine->stop();
    }

    return engine->newVariant(retval);
}



QScriptValue myPrintFunction(QScriptContext *context, QScriptEngine *engine)
{
    QString result;
    for (int i = 0; i < context->argumentCount(); ++i)
    {
        if (i > 0)
        {
            result.append(" ");
        }

        result.append(context->argument(i).toString());
    }

    QScriptValue calleeData = context->callee().data();
    QTextEdit *edit = qobject_cast<QTextEdit*>(calleeData.toQObject());
    edit->append("<i>" + result + "</i>");
    return engine->undefinedValue();
}

QScriptValue addToPlotFunction(QScriptContext *context, QScriptEngine *engine)
{
    QScriptValue calleeData = context->callee().data();
    ScriptEngine *myScriptEngine = qobject_cast<ScriptEngine*>(calleeData.toQObject());

    if (myScriptEngine && (context->argumentCount() == 2))
    {
        int module_id = context->argument(0).toInt32();
        int variable_id = context->argument(1).toInt32();

        qDebug() << "Should add plot variable id " << variable_id << " from module : " << module_id;

        return engine->newVariant(myScriptEngine->scopeRequest(module_id,variable_id));
    }
    else
    {
        return engine->newVariant(false);
    }
}


QScriptValue createPseudoModuleFunction(QScriptContext *context, QScriptEngine *engine)
{
    QScriptValue calleeData = context->callee().data();
    ScriptEngine *myScriptEngine = qobject_cast<ScriptEngine*>(calleeData.toQObject());

    if (myScriptEngine)
    {
        if (context->argumentCount() == 1)
        {
            int module_id = context->argument(0).toInt32();            
            return engine->newVariant(myScriptEngine->createPseudoModule(module_id));
        }
    }
    return engine->newVariant(false);
}

QScriptValue addScriptVariableFunction(QScriptContext *context, QScriptEngine *engine)
{
    QScriptValue calleeData = context->callee().data();
    ScriptEngine *myScriptEngine = qobject_cast<ScriptEngine*>(calleeData.toQObject());

    if (myScriptEngine)
    {
        if (context->argumentCount() == 3)
        {
            int module_id = context->argument(0).toInt32();
            QString name = context->argument(1).toString();
            QString description = context->argument(2).toString();

            return engine->newVariant(myScriptEngine->addScriptVariable(module_id,name,description));
        }
    }
    return engine->newVariant(false);
}


ScriptEngine::ScriptEngine(NetworkView *view)
    : BasePlugin(view), m_startTime(QTime::currentTime()), m_running(false), m_cycle(0)
{
	qDebug("ScriptEngine::ScriptEngine(NetworkView *view = %p)",view);

	//Setup UI
	m_ui.setupUi(this);


        //update all variables
        updateEngineVariables();

        //Connect modules add/remove
        connect(m_view,SIGNAL(moduleAdded(NetworkModule*)),this,SLOT(moduleAdded(NetworkModule*)));
        connect(m_view,SIGNAL(moduleRemoved(NetworkModule*)),this,SLOT(moduleRemoved(NetworkModule*)));

        //Connect buttons
        connect(m_ui.m_runButton,SIGNAL(clicked()),this,SLOT(runButtonClicked()));
        connect(m_ui.m_loadButton,SIGNAL(clicked()),this,SLOT(loadButtonClicked()));
	connect(m_ui.m_saveButton,SIGNAL(clicked()),this,SLOT(saveButtonClicked()));
        connect(m_ui.helpButton,SIGNAL(clicked()),this,SLOT(helpButtonClicked()));


	//Create timer
	m_timer = new QTimer(this);
	connect(m_timer,SIGNAL(timeout()),this,SLOT(timerUpdate()));



        //Period update
        connect(m_ui.spinBox,SIGNAL(valueChanged(int)),this,SLOT(updatePeriod(int)));

}

void ScriptEngine::init()
{

}

void ScriptEngine::terminate()
{

}

void ScriptEngine::timerUpdate()
{
    //qDebug("void ScriptEngine::timerUpdate()");
    QList<NetworkModule*> allModules = m_view->getModules();
    m_ui.lcdNumber->display(allModules.size());

    if (m_running)
    {
        //Update time
        QScriptValue timeValue = m_scriptEngine.newVariant((double) m_startTime.elapsed() / 1000.0);
        m_scriptEngine.globalObject().setProperty("time",timeValue,QScriptValue::ReadOnly);

        //Update period
        QScriptValue periodValue = m_scriptEngine.newVariant((double)m_ui.spinBox->value() / 1000.0);
        m_scriptEngine.globalObject().setProperty("period",periodValue,QScriptValue::ReadOnly);

        //Continue processing GUI events at 10ms interval
        m_scriptEngine.setProcessEventsInterval(10);

        QString code = m_ui.m_textEditScript->toPlainText();
        QScriptValue result = m_scriptEngine.evaluate(code);

        //Stop the script if it has errors, print them in the message box
        if (m_scriptEngine.hasUncaughtException())
        {
            //Output error in red
            QScriptValue errorObject = m_scriptEngine.uncaughtException();
            m_ui.m_textEditMessages->append("<span style=""color:red"">" + errorObject.toString() + " line: " + QString::number(m_scriptEngine.uncaughtExceptionLineNumber())+ "</span>");

            //Get backtrace
            QStringList backtrace = m_scriptEngine.uncaughtExceptionBacktrace();
            for (int i = 0; i < backtrace.size(); i++)
            {
               m_ui.m_textEditMessages->append("<span style=""color:blue"">" + backtrace[i] + "</span>");
            }


            //Do as if we have clicked the RUN/STOP button
            //runButtonClicked();
            m_running = false;

            qDebug("Stopping timer");
            m_timer->stop();
            m_ui.m_runButton->setText("RUN");
            m_ui.m_textEditScript->setEnabled(true);
        }
        else
        {
            m_ui.m_textEditMessages->append("Script returned with : " + result.toString());
        }

        //update cycle
        m_cycle++;
        QScriptValue cycleValue = m_scriptEngine.newVariant(m_cycle);
        m_scriptEngine.globalObject().setProperty("cycle",cycleValue,QScriptValue::ReadOnly);

    }
}


void ScriptEngine::loadButtonClicked()
{

    if (!m_running)
    {
        QString fileName = QFileDialog::getOpenFileName(NULL, "Open Script File",
                                                         m_lastPath,
                                                         "JavaScript (*.js)");
        if (fileName.size() > 0)
        {
                qDebug() << "Loading script :" << fileName;
                m_lastPath = fileName;

                QFile file(fileName);
                file.open(QIODevice::ReadOnly);
                m_ui.m_textEditScript->setText(file.readAll());
                file.close();

        }
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("Stop the script before loading a new one.");
        msgBox.exec();

    }
}

void ScriptEngine::saveButtonClicked()
{
	if (!m_running)
    {
        QString fileName = QFileDialog::getSaveFileName(NULL, "Save Script File",
														m_lastPath,
														"JavaScript (*.js)");
        if (fileName.size() > 0)
        {
			qDebug() << "Saving script :" << fileName;
			m_lastPath = fileName;
			QFile file(fileName);
			file.open(QIODevice::WriteOnly);
			QByteArray array = m_ui.m_textEditScript->toPlainText().toUtf8();
			file.write(array.data(),array.size());
			file.close();
        }
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("Stop the script before saving.");
        msgBox.exec();
		
    }
	
	
}

void ScriptEngine::runButtonClicked()
{
    //Update all variables
    updateEngineVariables();

    m_ui.m_textEditMessages->append(QString("<b>") + QDateTime::currentDateTime().toString() + "<\b>");

    //Special Case when timer = 0, running once
    if (m_ui.spinBox->value() == 0)
    {
        m_timer->stop();

        m_ui.m_textEditMessages->append("Starting once...");

        m_running = true;

        //reset elapsed
        m_startTime.start();
        timerUpdate();
        m_running = false;

        m_ui.m_runButton->setText("RUN");
        m_ui.m_textEditScript->setEnabled(true);
    }
    else
    {
        //Get the text from the textEdit
        if (m_running)
        {
            m_ui.m_textEditMessages->append("Stopping...");
            m_ui.m_runButton->setText("RUN");
            m_running = false;
            m_ui.m_textEditScript->setEnabled(true);

            qDebug("Stopping timer");
            m_timer->stop();
        }
        else
        {
            m_ui.m_textEditMessages->append("Starting...");
            m_ui.m_runButton->setText("STOP");
            m_ui.m_textEditScript->setEnabled(false);

            m_running = true;
            //reset elapsed
            m_startTime.start();

            qDebug("Starting timer");
            m_timer->start(m_ui.spinBox->value());
        }
    }
}

bool ScriptEngine::stop()
{
    if (m_running && m_ui.spinBox->value() != 0)
    {
        //Need to post an event to avoid recursive call to runButtonClicked()...
        QCoreApplication::postEvent(this,new StopScriptEvent());

        return true;
    }

    return false;
}

void ScriptEngine::updateEngineVariables(bool modulesOnly)
{
    Q_ASSERT(m_view);

    QList<NetworkModule*> allModules = m_view->getModules();

    //Update LCD   
    m_ui.lcdNumber->display(allModules.size());

    if (!modulesOnly)
    {
        //time
        QScriptValue timeValue = m_scriptEngine.newVariant((double)m_startTime.elapsed() / 1000.0);
        m_scriptEngine.globalObject().setProperty("time",timeValue,QScriptValue::ReadOnly);

        //period
        QScriptValue periodValue = m_scriptEngine.newVariant((double)m_ui.spinBox->value() / 1000.0);
        m_scriptEngine.globalObject().setProperty("period",periodValue,QScriptValue::ReadOnly);

        //Reset cycle counter
        m_cycle = 0;
        QScriptValue cycleValue = m_scriptEngine.newVariant(m_cycle);
        m_scriptEngine.globalObject().setProperty("cycle",cycleValue,QScriptValue::ReadOnly);

        //print function
        QScriptValue fun1 = m_scriptEngine.newFunction(myPrintFunction);
        fun1.setData(m_scriptEngine.newQObject(m_ui.m_textEditMessages));
        m_scriptEngine.globalObject().setProperty("print", fun1);

        //addToPlot function
        QScriptValue fun2 = m_scriptEngine.newFunction(addToPlotFunction);
        fun2.setData(m_scriptEngine.newQObject(this));
        m_scriptEngine.globalObject().setProperty("addToPlot", fun2);

        //createPseudoModule / addPseudoModule function
        QScriptValue fun3 = m_scriptEngine.newFunction(createPseudoModuleFunction);
        fun3.setData(m_scriptEngine.newQObject(this));
        m_scriptEngine.globalObject().setProperty("createPseudoModule",fun3);
        m_scriptEngine.globalObject().setProperty("addPseudoModule",fun3);

        //addScriptVariable function
        QScriptValue fun4 = m_scriptEngine.newFunction(addScriptVariableFunction);
        fun4.setData(m_scriptEngine.newQObject(this));
        m_scriptEngine.globalObject().setProperty("addScriptVariable",fun4);

        //addSliderControl function
        QScriptValue fun5 = m_scriptEngine.newFunction(addSliderControlFunction);
        fun5.setData(m_scriptEngine.newQObject(this));
        m_scriptEngine.globalObject().setProperty("addSliderControl",fun5);

        //stop function
        QScriptValue fun6 = m_scriptEngine.newFunction(stopFunction);
        fun6.setData(m_scriptEngine.newQObject(this));
        m_scriptEngine.globalObject().setProperty("stop",fun6);

    }

    //Modules & Variables
    QScriptValue moduleValue = m_scriptEngine.newObject();//m_scriptEngine.newArray(allModules.size());
    m_scriptEngine.globalObject().setProperty("modules",moduleValue);

    QScriptValue moduleArrayValue = m_scriptEngine.newArray(allModules.size());
    moduleValue.setProperty("array",moduleArrayValue);

    for (int mod = 0; mod < allModules.size(); mod++)
    {
        //Create module index variable
        NetworkModule *module = allModules[mod];
        Q_ASSERT(module);

        QScriptValue moduleObjectValue = m_scriptEngine.newQObject(module, QScriptEngine::QtOwnership, QScriptEngine::SkipMethodsInEnumeration);

        //Modules direct access
        moduleValue.setProperty(module->getConfiguration()->getDeviceID(),moduleObjectValue);

        //Array index values
        moduleArrayValue.setProperty(mod,moduleObjectValue);


        ModuleConfiguration *config = allModules[mod]->getConfiguration();
        Q_ASSERT(config);

        //Create variables array
        QScriptValue variableArrayValue = m_scriptEngine.newArray(config->size());
        moduleObjectValue.setProperty("variables",variableArrayValue);

        for (int i = 0; i < config->size(); i++)
        {
                //Making the variable available to the script engine
                ModuleVariable *var = (*config)[i];
                Q_ASSERT(var);
                QScriptValue objectValue = m_scriptEngine.newQObject(var,QScriptEngine::QtOwnership, QScriptEngine::SkipMethodsInEnumeration);

                objectValue.setProperty("name",var->getName(),QScriptValue::ReadOnly);
                objectValue.setProperty("type",var->getType(),QScriptValue::ReadOnly);
                objectValue.setProperty("DeviceID",var->getDeviceID(),QScriptValue::ReadOnly);
                objectValue.setProperty("description",var->getDescription(),QScriptValue::ReadOnly);
                objectValue.setProperty("offset",var->getOffset(),QScriptValue::ReadOnly);
                objectValue.setProperty("memType",var->getMemType(),QScriptValue::ReadOnly);
                objectValue.setProperty("activated",var->getActivated(),QScriptValue::ReadOnly);
                objectValue.setProperty("index",i,QScriptValue::ReadOnly);


                //Variables indexing
                variableArrayValue.setProperty(i,objectValue);

                //Module properties is each variable name
                moduleObjectValue.setProperty(var->getName(),objectValue);
        }

    }
}


void ScriptEngine::moduleAdded(NetworkModule *module)
{
    Q_ASSERT(module);

    //updating all variables & modules, but not the time
    updateEngineVariables(true);
}

void ScriptEngine::moduleRemoved(NetworkModule *module)
{
    Q_ASSERT(module);

    //updating all variables & modules, but not the time
    updateEngineVariables(true);
}

void ScriptEngine::updatePeriod(int period)
{
    m_timer->setInterval(period);
    m_ui.m_textEditMessages->append("<b>Period changed to : " + QString::number(period) + " ms </b>");

    if (period > 0)
        {m_ui.m_textEditMessages->append("<b>Timer Started.</b>");
        m_timer->start();        
    }
    else
    {
        m_running = false;
        m_ui.m_runButton->setText("RUN");
        m_ui.m_textEditScript->setEnabled(true);
        m_ui.m_textEditMessages->append("<b>Timer Stopped.</b>");
        m_timer->stop();
    }
}

bool ScriptEngine::scopeRequest(int module_id, int variable_id)
{
    NetworkModule* module = m_view->getModule(module_id);

    if (module)
    {
        ModuleVariable *var  = module->getVariable(variable_id);

        if (var)
        {
            m_view->scopeRequest(var);
            return true;
        }
    }

    return false;
}

void ScriptEngine::helpButtonClicked()
{
    m_view->helpWindowRequest("http://sourceforge.net/apps/mediawiki/openecosys/index.php?title=NetworkViewer:ScriptEngine");
}

bool ScriptEngine::createPseudoModule(int module_id)
{
    //Make sure the module does not exist
    if (!m_view->getModule(module_id) && module_id > 255)
    {
        ModuleConfiguration conf(-1,-1,-1,-1,-1,module_id);
        m_view->addModule(conf);

        updateEngineVariables(true);

        return true;
    }
    return false;
}


bool ScriptEngine::addScriptVariable(int module_id, QString name, QString description)
{
    NetworkModule *module = m_view->getModule(module_id);

    if (module)
    {
        //Make sure the name does not exist
        bool result = module->createScriptVariable(name,description);
        updateEngineVariables(true);
        return result;
    }

    return false;
}

bool ScriptEngine::addSliderControl(int module_id, int variable_id, double min_value, double max_value)
{
    NetworkModule* module = m_view->getModule(module_id);

    if (module)
    {
        ModuleVariable *var  = module->getVariable(variable_id);

        if (var)
        {
            QMdiSubWindow* subwindow = m_view->createSubWindow("Slider Control : " + var->getName() + " [" + QString::number(var->getDeviceID()) + "]");
            //Create the slider
            ModuleVariableSlider *slider = new ModuleVariableSlider(subwindow,var,min_value,max_value);
            //Add it to the window
            subwindow->setWidget(slider);
            //close window on slider destoy
            connect(slider,SIGNAL(terminateSlider()),subwindow,SLOT(close()));
            //Resize subwindow
            subwindow->resize(320,240);
            //Show subwindow
            subwindow->show();
            return true;
        }
    }

    return false;
}

bool ScriptEngine::event ( QEvent * e )
{
    if (e->type() == QEvent::User)
    {
        StopScriptEvent *myEvent = dynamic_cast<StopScriptEvent*>(e);

        if (myEvent)
        {
            runButtonClicked();
        }

        //accepting event
        e->accept();
        return true;
    }

    return QObject::event(e);
}

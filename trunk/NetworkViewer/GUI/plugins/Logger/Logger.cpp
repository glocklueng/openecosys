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

#include "Logger.h"
#include "NetworkView.h"
#include "ModuleVariableTableWidget.h"
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>

//This will insert the plugin in the dictionary...
static int logger_plugin_init = BasePlugin::registerPlugin("Logger",new BasePlugin::PluginFactory<Logger>());



Logger::Logger(NetworkView *view)
    : BasePlugin(view), m_logging(false)
{
    //qDebug("Logger::Logger(NetworkView *view = %p)",view);

    //Setup UI
    m_ui.setupUi(this);

    //Adding table widget
    m_table = new ModuleVariableTableWidget(this);
    m_ui.verticalLayout->addWidget(m_table);


    //Buttons
    connect(m_ui.pushButton_Start,SIGNAL(clicked()),this,SLOT(startButtonClicked()));
    connect(m_ui.pushButton_Stop,SIGNAL(clicked()),this,SLOT(stopButtonClicked()));
    connect(m_ui.pushButton_Save,SIGNAL(clicked()),this,SLOT(saveButtonClicked()));
    connect(m_ui.pushButton_Clear,SIGNAL(clicked()),this,SLOT(clearButtonClicked()));

    setLogging(false);
}

void Logger::init()
{
    qDebug("Logger::init()");
}

void Logger::terminate()
{
    qDebug("Logger::terminate()");
}


void Logger::startButtonClicked()
{

    setLogging(true);
}

void Logger::stopButtonClicked()
{
    setLogging(false);

}

void Logger::saveButtonClicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save CSV File"), ".", tr("CSV Text Files (*.csv)"));

    QFile file(fileName);
    file.open(QFile::WriteOnly);


    m_table->saveCSV(file);

}

void Logger::setLogging(bool state)
{
    //internal state
    QPalette palette = m_ui.label->palette();

    m_logging = state;

    //label
    if (state)
    {
        m_ui.label->setText("<b>Logging</b>");
        palette.setColor(m_ui.label->backgroundRole(),QColor(Qt::green));
    }
    else
    {

        m_ui.label->setText("<b>NOT Logging</b>");
        palette.setColor(m_ui.label->backgroundRole(),QColor(Qt::red));
    }

    m_table->setLogEnabled(m_logging);
    m_ui.label->setPalette(palette);
    m_ui.label->setAutoFillBackground(true);
    m_ui.label->update();
}

bool Logger::event ( QEvent * e )
{
    if(e->type() == BasePluginEvent::eventType())
    {

        BasePluginEvent *event = dynamic_cast<BasePluginEvent*>(e);

        if(event)
        {
            //Get the message...
            for(QMap<QString,QVariant>::iterator iter = event->m_map.begin(); iter != event->m_map.end(); iter++)
            {
                if (iter.key() == "addVariable")
                {
                    bool ok;
                    ModuleVariable *var = (ModuleVariable*) iter.value().toLongLong(&ok);
                    if (ok && var)
                    {
                        m_table->addVariable(var);
                    }
                }
                else if (iter.key() == "removeVariable")
                {
                    bool ok;
                    ModuleVariable *var = (ModuleVariable*) iter.value().toLongLong(&ok);
                    if (ok && var)
                    {
                        m_table->removeVariable(var);
                    }
                }
            }
        }

        e->accept();
        return true;
    }

    return BasePlugin::event(e);
}

void Logger::clearButtonClicked()
{
    m_table->clearLogs();
}



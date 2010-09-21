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

        //Connect signals
        connect(m_table,SIGNAL(variableAdded(ModuleVariable*)),this,SLOT(variableAdded(ModuleVariable*)));
        connect(m_table,SIGNAL(variableRemoved(ModuleVariable*)),this,SLOT(variableRemoved(ModuleVariable*)));

        //Buttons
        connect(m_ui.pushButton_Start,SIGNAL(clicked()),this,SLOT(startButtonClicked()));
        connect(m_ui.pushButton_Stop,SIGNAL(clicked()),this,SLOT(stopButtonClicked()));
        connect(m_ui.pushButton_File,SIGNAL(clicked()),this,SLOT(fileButtonClicked()));

        setLogging(false);
}

void Logger::init()
{

}

void Logger::terminate()
{

}

void Logger::variableRemoved(ModuleVariable* variable)
{
    //disconnect signals
    disconnect(variable,SIGNAL(valueChanged(ModuleVariable*)),this,SLOT(variableValueChanged(ModuleVariable*)));
    //Remove variable from list
    m_varList.removeAll(variable);
}

void Logger::variableAdded(ModuleVariable* variable)
{
    //connect variable modified signals
    connect(variable,SIGNAL(valueChanged(ModuleVariable*)),this,SLOT(variableValueChanged(ModuleVariable*)));
    //Add to variable list
    m_varList.push_back(variable);
}

void Logger::variableValueChanged(ModuleVariable* variable)
{
    //qDebug() << "Value changed for variable : " << variable->getName();

    if (m_logging)
    {
        QTextStream stream(&m_file);

        QDateTime myTime = QDateTime::currentDateTime();

        stream << (float) m_startTime.elapsed() / 1000.0 << "\t"
               << myTime.toString("dd.MM.yyyy") << "\t"
               << myTime.toString("hh:mm:ss.zzz") << "\t"
               << variable->getDeviceID() << "\t"
               << variable->getName() << "\t"
               << variable->getValue().toDouble() <<"\t"
               << "\n";

    }
}

void Logger::startButtonClicked()
{

    m_startTime = QTime::currentTime();


    m_startTime.start();

    //Test if we have selected the file name
    if (m_ui.lineEdit->text().size() == 0)
    {
        QMessageBox msgBox;
        msgBox.setText("Please select a file name first.");
        msgBox.exec();

        return;
    }

    setLogging(true);
}

void Logger::stopButtonClicked()
{
    setLogging(false);

}

void Logger::fileButtonClicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save File"), ".", tr("Text Files (*.txt)"));


    //make sure we close the previously opened file
    m_file.close();

    m_file.setFileName(fileName);

    //Open file for writing
    m_file.open(QIODevice::WriteOnly);

    //Change Line Edit
    m_ui.lineEdit->setText(fileName);


    //Write file header
    QTextStream stream(&m_file);

    stream << "ELAPSED" << "\t"
           << "DATE" << "\t"
           << "TIME" << "\t"
           << "DEVICE" << "\t"
           << "VARIABLE" << "\t"
           << "VALUE" <<"\t"
           << "\n";
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

    m_ui.label->setPalette(palette);
    m_ui.label->setAutoFillBackground(true);
    m_ui.label->update();
}

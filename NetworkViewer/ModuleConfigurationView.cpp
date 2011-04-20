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

#include "ModuleConfigurationView.h"
#include <QCheckBox>
#include <QFileDialog>



ModuleConfigurationView::ModuleConfigurationView(QWidget *parent, NetworkModule *module)
    : QWidget(parent), m_module(module)
{
    Q_ASSERT(m_module);

    //Setup UI
    m_ui.setupUi(this);

    //Create table widget, interactive, into UI frame
    m_table = new ModuleVariableTableWidget(this,true);

    //Add the table to the layout
    m_ui.verticalLayout->addWidget(m_table);

    //Make sure we know when the module is destroyed
    connect(m_module,SIGNAL(destroyed()),this,SLOT(moduleDestroyed()));

    //Add module variables
    for (unsigned int i = 0; i < m_module->getNumVariable(); i++)
    {
        m_table->addVariable(m_module->getVariable(i));
    }

    //Make sure everything fits
    m_table->resizeColumnsToContents();
    m_table->resizeRowsToContents();

    //ModuleConfiguration Signals
    connect(m_module->getConfiguration(), SIGNAL(variableAdded(ModuleVariable*)),this,SLOT(ModuleVariableAdded(ModuleVariable*)));
    connect(m_module->getConfiguration(), SIGNAL(variableRemoved(ModuleVariable*)),this,SLOT(ModuleVariableRemoved(ModuleVariable*)));
    connect(m_module->getConfiguration(),SIGNAL(configurationAboutToLoad()),this,SLOT(configurationAboutToLoad()));

    //TableView signals.
    connect(m_table,SIGNAL(cellChanged(int,int)),this,SLOT(cellChanged(int,int)));
    connect(m_table,SIGNAL(cellDoubleClicked(int,int)),SLOT(cellDoubleClicked(int,int)));
    connect(m_table,SIGNAL(variableRemoved(ModuleVariable*)),this,SLOT(ModuleVariableDeleted(ModuleVariable*)));


    //Buttons
    connect(m_ui.toolButton_Activate,SIGNAL(clicked()),this,SLOT(activateAllVariables()));
    connect(m_ui.toolButton_Deactivate,SIGNAL(clicked()),this,SLOT(disableAllVariables()));
    connect(m_ui.toolButton_SaveConf,SIGNAL(clicked()),this,SLOT(saveConfiguration()));
    connect(m_ui.toolButton_LoadConf,SIGNAL(clicked()),this,SLOT(loadConfiguration()));

}



void ModuleConfigurationView::ModuleVariableAdded(ModuleVariable *var)
{    
    m_table->addVariable(var);
}

void ModuleConfigurationView::ModuleVariableRemoved(ModuleVariable *var)
{
    m_table->removeVariable(var);
}

void ModuleConfigurationView::ModuleVariableDeleted(ModuleVariable *var)
{
    //The user have deleted a variable
    m_module->getConfiguration()->removeVariable(var);
}


void ModuleConfigurationView::moduleDestroyed()
{
    qDebug("ModuleConfigurationView::moduleDestroyed()");
    m_module = NULL;
    emit closeRequest();
}

void ModuleConfigurationView::cellChanged ( int row, int column )
{
    //qDebug("ModuleVariableTableWidget::cellChanged row %i col %i",row,column);

    if (m_module)
    {
        ModuleConfiguration *config = m_module->getConfiguration();
        Q_ASSERT(config);

        if (column == ModuleVariableTableWidget::VARIABLE_ACTIVATED)
        {
            QCheckBox *checkbox = dynamic_cast<QCheckBox*>(m_table->cellWidget(row,ModuleVariableTableWidget::VARIABLE_ACTIVATED));
            if (checkbox && (row < config->size()))
            {
                (*config)[row]->setActivated(checkbox->isChecked());
            }
        }
        else if (column == ModuleVariableTableWidget::VARIABLE_VALUE) //Value modification from user...
        {
            QTableWidgetItem * myItem= m_table->item(row,column);
            QString text = myItem->text();

            //Important to call setUserValue so the system can know
            //It has been updated by user input
            if (row < config->size())
            {
                (*config)[row]->setValue(text,true);
            }
        }
    }
}

void ModuleConfigurationView::cellDoubleClicked(int row, int column)
{
    //qDebug("ModuleVariableTableWidget::cellDoubleClicked row %i col %i",row,column);

    if (column == ModuleVariableTableWidget::VARIABLE_NAME && m_module)
    {
        ModuleConfiguration *config = m_module->getConfiguration();
        Q_ASSERT(config);

        if (row < config->size())
        {
            emit scopeRequest((*config)[row]);
        }
    }
}

void ModuleConfigurationView::configurationAboutToLoad()
{
    qDebug("ModuleConfigurationView::configurationAboutToLoad()");

    //Just Have to cleanup rapidly, we will have new variables after.
    //m_table->reset();

}

void ModuleConfigurationView::activateAllVariables()
{
    if(m_module)
    {
        ModuleConfiguration *config = m_module->getConfiguration();
        for (unsigned int i = 0; i < config->size(); i++)
        {
            (*config)[i]->setActivated(true);
        }
    }
}

void ModuleConfigurationView::disableAllVariables()
{
    if (m_module)
    {
        ModuleConfiguration *config = m_module->getConfiguration();
        for (unsigned int i = 0; i < config->size(); i++)
        {
            (*config)[i]->setActivated(false);
        }
    }
}

void ModuleConfigurationView::loadConfiguration()
{

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Configuration File"),
                                                     m_module->getConfiguration()->getFilename(),
                                                     "XML (*.xml)");
    if (fileName.size() > 0)
    {
            qDebug() << "Loading configuration :" << fileName;
            //Loading config, variables only
            m_module->getConfiguration()->loadConfiguration(fileName,true);
    }
}

void ModuleConfigurationView::saveConfiguration()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Configuration File"),
                                                    m_module->getConfiguration()->getFilename(),
                                                    "XML (*.xml)");
    if (fileName.size() > 0)
    {
            qDebug() << "Saving configuration :" << fileName;
            m_module->getConfiguration()->saveConfiguration(fileName);
    }
}

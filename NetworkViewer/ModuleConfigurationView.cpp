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




ModuleConfigurationView::ModuleConfigurationView(QWidget *parent, NetworkModule *module)
    : ModuleVariableTableWidget(parent, true), m_module(module)
{
    Q_ASSERT(m_module);

    connect(m_module,SIGNAL(destroyed()),this,SLOT(moduleDestroyed()));


    for (unsigned int i = 0; i < m_module->getNumVariable(); i++)
    {
        addVariable(m_module->getVariable(i));
    }

    //Make sure everything fits
    resizeColumnsToContents();
    resizeRowsToContents();

    //ModuleConfiguration Signals
    connect(m_module->getConfiguration(), SIGNAL(variableAdded(ModuleVariable*)),this,SLOT(ModuleVariableAdded(ModuleVariable*)));
    connect(m_module->getConfiguration(), SIGNAL(variableRemoved(ModuleVariable*)),this,SLOT(ModuleVariableRemoved(ModuleVariable*)));
    connect(m_module->getConfiguration(),SIGNAL(configurationAboutToLoad()),this,SLOT(configurationAboutToLoad()));

    //TableView signals.
    connect(this,SIGNAL(cellChanged(int,int)),this,SLOT(cellChanged(int,int)));
    connect(this,SIGNAL(cellDoubleClicked(int,int)),SLOT(cellDoubleClicked(int,int)));
    connect(this,SIGNAL(variableRemoved(ModuleVariable*)),this,SLOT(ModuleVariableDeleted(ModuleVariable*)));

}


void ModuleConfigurationView::ModuleVariableAdded(ModuleVariable *var)
{    
    addVariable(var);
}

void ModuleConfigurationView::ModuleVariableRemoved(ModuleVariable *var)
{
    removeVariable(var);
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

    if (m_module && m_interactive)
    {
        ModuleConfiguration *config = m_module->getConfiguration();
        Q_ASSERT(config);

        if (column == VARIABLE_VALUE) //Value modification from user...
        {
            QTableWidgetItem * myItem= item(row,column);
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

    if (column == VARIABLE_NAME && m_module && m_interactive)
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
    m_variableMap.clear();

    //Clear table
    clear();
    setRowCount(0);

}

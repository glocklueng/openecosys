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
#include "ModuleVariableTableWidget.h"

//Indexes of table view
enum {VARIABLE_ACTIVATED, VARIABLE_NAME, VARIABLE_VALUE_TYPE, VARIABLE_MEMORY_TYPE, VARIABLE_MEMORY_OFFSET, VARIABLE_VALUE, VARIABLE_DESCRIPTION};

ModuleConfigurationView::ModuleConfigurationView(NetworkModule *module)
    :	m_module(module)
{


	Q_ASSERT(m_module);

	connect(m_module,SIGNAL(destroyed()),this,SLOT(moduleDestroyed()));

        setupUi(this);

	updateConfiguration();

	//Connect signals after updating avoiding causing backward modification
	connect(m_table,SIGNAL(cellChanged(int,int)),this,SLOT(cellChanged(int,int)));
	connect(m_table,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(cellDoubleClicked(int, int)));

        connect(toolButton_Activate,SIGNAL(clicked()),this,SLOT(activateAllClicked()));
        connect(toolButton_Deactivate,SIGNAL(clicked()),this,SLOT(deactivateAllClicked()));
        connect(m_module->getConfiguration(),SIGNAL(variableAdded(ModuleVariable*)),this, SLOT(variableAdded(ModuleVariable*)));

}

void ModuleConfigurationView::updateConfiguration()
{
	if (m_module)
	{
		updateView();

		ModuleConfiguration *config = m_module->getConfiguration();
		Q_ASSERT(config);
		

		for (int i = 0; i < config->size(); i++)
		{
			ModuleVariable *var = (*config)[i];
			connect(var,SIGNAL(valueChanged(ModuleVariable*)),this,SLOT(variableUpdated(ModuleVariable*)));
		}
	
	}
	else
	{
		qWarning("ModuleConfigurationView::updateConfiguration() called on NULL module");
	}
}

void ModuleConfigurationView::variableUpdated(ModuleVariable *variable)
{
	if (m_module)
	{

		//qDebug("ModuleConfigurationView::variableUpdated(ModuleVariable &variable)");
		ModuleConfiguration *config = m_module->getConfiguration();

		Q_ASSERT(config);
		
		//This will make sure we don't go into recursive updates
		for (int i = 0; i < config->size(); i++)
		{
			//This will compare variable name & version
			if (variable == (*config)[i])
			{
				//Get the item...
				QTableWidgetItem* item = m_table->item(i,VARIABLE_VALUE);


				if (item)
				{
					//Disable signals from m_table while updating
					//If we dont do that, will constantly try to update the variable
					disconnect(m_table,SIGNAL(cellChanged(int,int)),this,SLOT(cellChanged(int,int)));
					item->setText(variable->getValue().toString());
					connect(m_table,SIGNAL(cellChanged(int,int)),this,SLOT(cellChanged(int,int)));
				}
			}
		}
	}
	else
	{
		qWarning("ModuleConfigurationView::variableUpdated(ModuleVariable *variable = %p) called on NULL module",variable);
	}
}


void ModuleConfigurationView::cellChanged ( int row, int column )
{
	qDebug("ModuleConfigurationView::cellChanged row %i col %i",row,column);
	
	if (m_module)
	{
		ModuleConfiguration *config = m_module->getConfiguration();
		Q_ASSERT(config);
		
		if (column == VARIABLE_VALUE) //Value modification from user...
		{
			QTableWidgetItem * item = m_table->item(row,column);
			QString text = item->text();

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
	qDebug("ModuleConfigurationView::cellDoubleClicked row %i col %i",row,column);

	if (column == VARIABLE_NAME && m_module)
	{
		ModuleConfiguration *config = m_module->getConfiguration();
		Q_ASSERT(config);
		
		if (row < config->size())
		{
			emit scopeRequest((*config)[row]);
		}
	}
}

void ModuleConfigurationView::updateView()
{
	qDebug("ModuleConfigurationView::updateView()");
	
	if (m_module)
	{
		ModuleConfiguration *config = m_module->getConfiguration();
		Q_ASSERT(config);
	
		//Resize table first
                m_table->clear();
		m_table->setRowCount(config->size());
		
		//Reconstruct the table...
		for (int i = 0; i < config->size(); i++)
		{
			ModuleVariable *variable = (*config)[i];

			//Name
			QTableWidgetItem *nameItem = new QTableWidgetItem(variable->getName());
			nameItem->setFlags(Qt::ItemIsEnabled);
                        nameItem->setIcon(QIcon(":/images/add.png"));
                        nameItem->setFlags(Qt::ItemIsDragEnabled | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
                        //nameItem->setData(Qt::UserRole,QVariant(variable->getName()));
                        nameItem->setData(Qt::UserRole,QVariant((unsigned long long)variable));
                        m_table->setItem (i, VARIABLE_NAME,nameItem);

			//ValueType
			QTableWidgetItem *typeItem = new QTableWidgetItem(ModuleVariable::typeToString(variable->getType()));
			typeItem->setFlags(Qt::ItemIsEnabled);
			m_table->setItem (i, VARIABLE_VALUE_TYPE,typeItem);

			
			//MemType
			QTableWidgetItem *memTypeItem = NULL;
			switch (variable->getMemType())
			{
				case ModuleVariable::RAM_VARIABLE:
					memTypeItem = new QTableWidgetItem("RAM");
				break;
				
				case ModuleVariable::EEPROM_VARIABLE:
					memTypeItem = new QTableWidgetItem("EEPROM");
				break;

                                case ModuleVariable::SCRIPT_VARIABLE:
                                        memTypeItem = new QTableWidgetItem("SCRIPT");
                                break;
			
			}
			memTypeItem->setFlags(Qt::ItemIsEnabled);
			m_table->setItem (i, VARIABLE_MEMORY_TYPE,memTypeItem);
			
			//Offset
			QTableWidgetItem *offsetItem = new QTableWidgetItem(QString::number(variable->getOffset()));
			offsetItem->setFlags(Qt::ItemIsEnabled);
			m_table->setItem (i, VARIABLE_MEMORY_OFFSET,offsetItem);

			//Value
			QTableWidgetItem *valueItem = new QTableWidgetItem(variable->getValue().toString());
			valueItem->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled);
			//TODO SET VALIDATOR FOR TEXT ENTRY ACCORDING TO VARIABLE TYPE...
			m_table->setItem (i, VARIABLE_VALUE,valueItem);


			//Description
			QTableWidgetItem *descriptionItem = new QTableWidgetItem(variable->getDescription());
			descriptionItem->setFlags(Qt::ItemIsEnabled);
			m_table->setItem (i, VARIABLE_DESCRIPTION,descriptionItem);


            //Activated
            QCheckBox *activatedCheckBox = new QCheckBox(m_table);
            activatedCheckBox->setChecked(variable->getActivated());
            m_table->setCellWidget(i,VARIABLE_ACTIVATED,activatedCheckBox);
            connect(activatedCheckBox,SIGNAL(toggled(bool)),variable,SLOT(setActivated(bool)));
            connect(variable,SIGNAL(variableActivated(bool)),activatedCheckBox,SLOT(setChecked(bool)));

		}
		
		m_table->resizeColumnsToContents();
	}
	else
	{
		qWarning("ModuleConfigurationView::updateView() called on NULL module");
	}
}




void ModuleConfigurationView::moduleDestroyed()
{
	qDebug("ModuleConfigurationView::moduleDestroyed()");
	m_module = NULL;
	emit closeRequest();
}

void ModuleConfigurationView::activateAllClicked()
{
    ModuleConfiguration *conf = m_module->getConfiguration();

    for (int i = 0; i < conf->size(); i++)
    {
        (*conf)[i]->setActivated(true);
    }

}

void ModuleConfigurationView::deactivateAllClicked()
{
    ModuleConfiguration *conf = m_module->getConfiguration();

    for (int i = 0; i < conf->size(); i++)
    {
        (*conf)[i]->setActivated(false);
    }
}


 void ModuleConfigurationView::variableAdded(ModuleVariable *variable)
 {
    updateView();
 }

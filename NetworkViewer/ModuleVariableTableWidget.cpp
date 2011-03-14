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

#include "ModuleVariableTableWidget.h"
#include <QDropEvent>
#include <QTableWidgetItem>
#include <QByteArray>
#include <QDataStream>
#include <QMimeData>
#include <QDrag>
#include <QCheckBox>



ModuleVariableTableWidget::ModuleVariableTableWidget(QWidget *parent, bool interactive)
    : QTableWidget(parent), m_interactive(interactive)
{

    //Will accept drop by default  
    setAcceptDrops(true);

    if (m_interactive)
    {
        setDragEnabled(true);
    }

    //Set Columns size & labels
    setColumnCount(VARIABLE_ENUM_SIZE);

    QStringList labels;
    labels << "Activated" << "Name" << "ValueType" << "MemoryType" << "Memory Offset" << "Value" << "Description";
    setHorizontalHeaderLabels(labels);

}


void ModuleVariableTableWidget::dropEvent(QDropEvent *event)
{
    qDebug("ModuleVariableTableWidget::dropEvent(QDropEvent *event = %p)",event);
    qDebug()<< event->mimeData()->text();
    qDebug()<< event->mimeData()->formats();

    QByteArray data = event->mimeData()->data("application/x-qabstractitemmodeldatalist");

    qDebug("data size : %i",data.size());
    QDataStream stream(&data, QIODevice::ReadOnly);

    while (!stream.atEnd())
    {
        int row, col;
        QMap<int,  QVariant> roleDataMap;
        stream >> row >> col >> roleDataMap;

        qDebug("got something from row: %i, col: %i",row,col);

        if (roleDataMap.contains(Qt::UserRole))
        {
            qDebug() << "Found : " << roleDataMap[Qt::UserRole];

            ModuleVariable *variable = (ModuleVariable*) roleDataMap[Qt::UserRole].toULongLong();

            qDebug("got pointer : %p",variable);

            addVariable(variable);
        }

    }
}


bool ModuleVariableTableWidget::removeVariable(ModuleVariable *variable)
{
    if(m_variableMap.contains(variable))
    {
        //Remove Table index
        int index = m_variableMap[variable];

        this->removeRow(index);

        //Remove from map
        m_variableMap.remove(variable);

        emit variableRemoved(variable);


        //Re-index map
        for (QMap<ModuleVariable*,int>::iterator iter = m_variableMap.begin(); iter != m_variableMap.end(); iter++)
        {
            if (iter.value() > index)
            {                
                m_variableMap[iter.key()] = iter.value() - 1;
            }
        }

        return true;
    }

    return false;
}

void ModuleVariableTableWidget::variableDestroyed(ModuleVariable *variable)
{
    removeVariable(variable);
}

void ModuleVariableTableWidget::variableActivated(bool state, ModuleVariable *variable)
{
    if (m_variableMap.contains(variable))
    {
        int index = m_variableMap[variable];

        QCheckBox *checkbox = dynamic_cast<QCheckBox*>(this->cellWidget(index,VARIABLE_ACTIVATED));

        if(checkbox)
        {
            checkbox->setChecked(state);
        }
        else
        {
            qWarning("ModuleVariableTableWidget::variableActivated -- Widget not found at : %i, %i",index,VARIABLE_VALUE);
        }
    }
}


void ModuleVariableTableWidget::variableValueChanged(ModuleVariable *variable)
{
    if (m_variableMap.contains(variable))
    {
        int index = m_variableMap[variable];

        //qDebug("value changed with index :%i",index);

        QTableWidgetItem *item = this->item(index,VARIABLE_VALUE);

        if (item)
        {
            //Make sure we do not emit cell changed when we update the variable, avoiding a write/request write loop.
            blockSignals(true);
            item->setText(variable->getValue().toString());
            blockSignals(false);
        }
        else
        {
            qWarning("ModuleVariableTableWidget::variableValueChanged -- Item not found at : %i, %i",index,VARIABLE_VALUE);
        }
    }
}

bool ModuleVariableTableWidget::addVariable(ModuleVariable *variable)
{
    //Variable already exist?
    if (m_variableMap.contains(variable))
    {
        return false;
    }

    blockSignals(true);

    //Add one row
    int index = rowCount();
    setRowCount(index + 1);

    //Add to map
    m_variableMap[variable] = index;

    //Connect destroyed signal
    connect(variable,SIGNAL(aboutToDestroy(ModuleVariable*)),this,SLOT(variableDestroyed(ModuleVariable*)));

    //Connect value changed signal
    connect(variable,SIGNAL(valueChanged(ModuleVariable*)),this,SLOT(variableValueChanged(ModuleVariable*)));

    //Connect variable activated
    connect(variable,SIGNAL(variableActivated(bool,ModuleVariable*)),this,SLOT(variableActivated(bool,ModuleVariable*)));

    //Name
    QTableWidgetItem *nameItem = new QTableWidgetItem(variable->getName());
    nameItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsDragEnabled | Qt::ItemIsSelectable);
    nameItem->setIcon(QIcon(":/images/add.png"));
    nameItem->setData(Qt::UserRole,QVariant((unsigned long long)variable));
    setItem (index, VARIABLE_NAME,nameItem);

    //ValueType
    QTableWidgetItem *typeItem = new QTableWidgetItem(ModuleVariable::typeToString(variable->getType()));
    typeItem->setFlags(Qt::ItemIsEnabled);
    setItem (index, VARIABLE_VALUE_TYPE,typeItem);


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
    setItem (index, VARIABLE_MEMORY_TYPE,memTypeItem);

    //Offset
    QTableWidgetItem *offsetItem = new QTableWidgetItem(QString::number(variable->getOffset()));
    offsetItem->setFlags(Qt::ItemIsEnabled);
    setItem (index, VARIABLE_MEMORY_OFFSET,offsetItem);

    //Value
    QTableWidgetItem *valueItem = new QTableWidgetItem(variable->getValue().toString());
    if (m_interactive)
    {
        valueItem->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled);
    }
    else
    {
         valueItem->setFlags(Qt::ItemIsEnabled);
    }
    //TODO SET VALIDATOR FOR TEXT ENTRY ACCORDING TO VARIABLE TYPE...
    setItem (index, VARIABLE_VALUE,valueItem);

    //Description
    QTableWidgetItem *descriptionItem = new QTableWidgetItem(variable->getDescription());
    descriptionItem->setFlags(Qt::ItemIsEnabled);
    setItem (index, VARIABLE_DESCRIPTION,descriptionItem);

    //Activated
    QCheckBox *activatedCheckBox = new QCheckBox(this);
    activatedCheckBox->setEnabled(m_interactive);
    activatedCheckBox->setChecked(variable->getActivated());
    this->setCellWidget(index,VARIABLE_ACTIVATED,activatedCheckBox);

    //Make sure everything is visible
    resizeRowsToContents();
    resizeColumnsToContents();

    blockSignals(false);

    emit variableAdded(variable);

    return true;
}

void ModuleVariableTableWidget::keyPressEvent ( QKeyEvent * event )
{
    //qDebug("ModuleVariableTableWidget::keyPressEvent ( QKeyEvent * event )");

    //Delete key handling
    if (event->key() == Qt::Key_Backspace || event->key() == Qt::Key_Delete)
    {
        //qDebug("Delete or backspace pressed at row %i.",currentRow());
        int row = currentRow();


        //reverse lookup for variable at current row
        for(QMap<ModuleVariable*,int>::iterator iter = m_variableMap.begin(); iter != m_variableMap.end(); iter++)
        {
            if (iter.value() == row)
            {
                removeVariable(iter.key());                
                break;
            }
        }
    }
    else
    {
        //Default implementation
        QTableWidget::keyPressEvent(event);
    }
}



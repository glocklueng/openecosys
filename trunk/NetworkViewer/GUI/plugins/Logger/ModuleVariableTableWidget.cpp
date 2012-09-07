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
#include <QHeaderView>
#include <QTextStream>


ModuleVariableTableWidget::ModuleVariableTableWidget(QWidget *parent)
    : QTableWidget(parent), m_logEnabled(false)
{

    //Will accept drop by default
    setAcceptDrops(true);


    setDragEnabled(true);


    //Set Columns size & labels
    setColumnCount(VARIABLE_ENUM_SIZE);

    QStringList labels;


    labels << "ModuleID" << "Name" << "ValueType" << "Value" << "Log Count";
    setHorizontalHeaderLabels(labels);


    //Make sure everything fits
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    setWordWrap(true);

    //Make sure everything is visible
    resizeColumnsToContents();

    //Set maximum width of column
    resizeRowsToContents();

    //Streching
    horizontalHeader()->setStretchLastSection(true);
    //verticalHeader()->setStretchLastSection(true);
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


bool ModuleVariableTableWidget::removeVariable(ModuleVariable *variable,  bool emitSignal)
{
    if(m_variableMap.contains(variable))
    {
        //Remove Table index
        int index = m_variableMap[variable];

        this->removeRow(index);

        //Remove from map
        m_variableMap.remove(variable);

        if (emitSignal)
        {
            emit variableRemoved(variable);
        }

        //Re-index map
        for (QMap<ModuleVariable*,int>::iterator iter = m_variableMap.begin(); iter != m_variableMap.end(); iter++)
        {
            if (iter.value() > index)
            {
                m_variableMap[iter.key()] = iter.value() - 1;
            }
        }

        //Make sure everything is visible
        //resizeRowsToContents();
        //resizeColumnToContents();


        return true;
    }

    return false;
}

void ModuleVariableTableWidget::variableDestroyed(ModuleVariable *variable)
{
    removeVariable(variable,false);
}




void ModuleVariableTableWidget::variableValueChanged(ModuleVariable *variable)
{
    if (m_variableMap.contains(variable))
    {
        int index = m_variableMap[variable];

        if (m_logEnabled)
        {
            m_logValues[variable].push_back(QPair<QDateTime,QVariant>(variable->getUpdateTime(),variable->getValue()));
        }

        QTableWidgetItem *item_value = this->item(index,VARIABLE_VALUE);
        QTableWidgetItem *item_count = this->item(index,VARIABLE_LOG_COUNT);


        if (item_value && item_count)
        {
            //Make sure we do not emit cell changed when we update the variable, avoiding a write/request write loop.
            blockSignals(true);
            item_value->setText(variable->getValue().toString());
            item_count->setText(QString::number(m_logValues[variable].size()));
            //Make sure everything is visible
            //resizeRowsToContents();
            //resizeColumnsToContents();

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
    m_logValues[variable] = QList<QPair<QDateTime,QVariant> >(); //empty list

    //Connect destroyed signal
    connect(variable,SIGNAL(aboutToDestroy(ModuleVariable*)),this,SLOT(variableDestroyed(ModuleVariable*)));

    //Connect value changed signal
    connect(variable,SIGNAL(valueChanged(ModuleVariable*)),this,SLOT(variableValueChanged(ModuleVariable*)));


    //Module ID
    QTableWidgetItem *moduleIDItem =  new QTableWidgetItem(QString::number(variable->getDeviceID()));
    moduleIDItem->setFlags(Qt::ItemIsEnabled);
    setItem(index,VARIABLE_MODULE_ID,moduleIDItem);


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

    //Value
    QTableWidgetItem *valueItem = new QTableWidgetItem(variable->getValue().toString());
    valueItem->setFlags(Qt::ItemIsEnabled);
    setItem (index, VARIABLE_VALUE,valueItem);


    //Log Counter
    QTableWidgetItem *logCountItem = new QTableWidgetItem(QString::number(m_logValues[variable].size()));
    setItem(index, VARIABLE_LOG_COUNT, logCountItem);


    //Make sure everything is visible
    resizeColumnsToContents();
    blockSignals(false);


    resizeRowsToContents();
    horizontalHeader()->setStretchLastSection(true);

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

        if (event->key() == Qt::Key_Enter)
        {
            int row = currentRow();
            int col = currentColumn();

            if (col == VARIABLE_VALUE)
            {
                qDebug("enterPressed %i,%i",row,col);
                emit enterPressed(row,col);
            }
        }
    }
}

void ModuleVariableTableWidget::clearContents()
{
    blockSignals(true);
    m_variableMap.clear();
    m_logValues.clear();

    //Clear table
    clear();
    setRowCount(0);
    blockSignals(false);
}

void ModuleVariableTableWidget::showEvent (QShowEvent * event)
{
    //qDebug("ModuleVariableTableWidget::showEvent (QShowEvent * event)");
    resizeRowsToContents();
    QTableWidget::showEvent(event);
}

void ModuleVariableTableWidget::setLogEnabled(bool enabled)
{
    m_logEnabled = enabled;
}

void ModuleVariableTableWidget::clearLogs()
{
    m_logValues.clear();

    //Reset counters
    for (unsigned int i = 0; i < rowCount(); i++)
    {
        QTableWidgetItem *item_count = this->item(i,VARIABLE_LOG_COUNT);

        if (item_count)
        {
            item_count->setText(QString::number(0));
        }

    }
}

bool ModuleVariableTableWidget::saveCSV(QIODevice &output)
{
    if (output.isWritable())
    {
        for ( QMap<ModuleVariable*, QList<QPair<QDateTime, QVariant> > >::iterator iter = m_logValues.begin(); iter != m_logValues.end(); iter++)
        {
            QTextStream stream(&output);

            //Something to write ?
            if (iter.value().size() > 0)
            {

                //Write Header
                stream << "ModuleID"<<"\t"<<"Variable"<<"\t"<<"Date" <<"\t"<<"Time" << "\t" << "Value" << "\n";

                //Write time / value pair
                for (unsigned int i= 0; i < iter.value().size(); i++)
                {
                    QDateTime time = iter.value()[i].first;
                    QVariant value = iter.value()[i].second;

                    stream << iter.key()->getDeviceID() << "\t";
                    stream << iter.key()->getName() << "\t";
                    stream << time.toString("dd/MM/yyyy\thh:mm:ss.zzz") << "\t" <<  value.toString() << "\n";
                }

                stream << "\n\n";
            }
        }
        return true;
    }

    return false;
}

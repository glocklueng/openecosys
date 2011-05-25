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


#ifndef _MODULE_CONFIGURATION_VIEW_H_
#define _MODULE_CONFIGURATION_VIEW_H_


#include "NetworkModule.h"
#include <QScriptEngine>
#include <QScriptEngineAgent>
#include <QTimer>
#include <QTime>
#include "ModuleVariableTableWidget.h"
#include "ui_ModuleConfigurationView.h"

class ModuleConfigurationView : public QWidget
{
    Q_OBJECT;

public:

    ModuleConfigurationView(QWidget *parent, NetworkModule *module);

signals:

    /**
        This signal will be emitted when the user closes the view
    */
    void closeRequest();

    /**
        This signal will be emitted when the user clicks on a variable
        \param variable The variable requested
    */
    void scopeRequest(ModuleVariable *variable);

protected slots:

    //void ModuleVariableAdded(ModuleVariable *var);
    //void ModuleVariableRemoved(ModuleVariable *var);
    //void ModuleVariableDeleted(ModuleVariable *var);

    /**
        Called when the associated NetworkModule is destroyed.
    */
    void moduleDestroyed();

    //void cellChanged ( int row, int column );
    void cellDoubleClicked(const QModelIndex &index);
    //void configurationAboutToLoad();

    void activateAllVariables();
    void disableAllVariables();
    void loadConfiguration();
    void saveConfiguration();
    void newVariableClicked();
	
protected:

    NetworkModule *m_module;
    //ModuleVariableTableWidget *m_table;
    Ui::ModuleConfigurationView m_ui;
    QTableView *m_table;


};

#endif


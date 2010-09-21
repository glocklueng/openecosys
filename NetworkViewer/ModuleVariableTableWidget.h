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

#ifndef _MODULE_VARIABLE_TABLE_WIDGET_
#define _MODULE_VARIABLE_TABLE_WIDGET_

#include <QTableWidget>
#include "NetworkModule.h"
#include <QMap>


/**
    Modified TableWidget to display module variables
    \author Dominic Letourneau
*/
class ModuleVariableTableWidget : public QTableWidget
{
    Q_OBJECT;

public:

    /**
        Constructor
        \param parent The parent Widget
    */
    ModuleVariableTableWidget(QWidget *parent = NULL);

signals:

    /**
        Emit this signal when a variable is added
        \param variable The variable
    */
    void variableAdded(ModuleVariable *variable);


    /**
        Emit this signal when a variable is removed
        \param variable the variable
    */
    void variableRemoved(ModuleVariable *variable);

protected slots:

    /**
        Called when a variable is about to be destroyed
        \param variable the variable
    */
    void variableDestroyed(ModuleVariable *variable);

    /**
        Called when a variable's value change
        \param variable the variable
    */
    void variableValueChanged(ModuleVariable *variable);

    /**
        Called when a variable is activated
        \param state Activation state
        \param variable the variable
    */
    void variableActivated(bool state, ModuleVariable *variable);

protected:
    ///Internal add variable
    bool addVariable(ModuleVariable *var);
    ///Internal remove variable
    bool removeVariable(ModuleVariable *var);
    ///Internal drop event handler
    virtual void dropEvent(QDropEvent *event);
    ///Handling keyboard interaction
    virtual void keyPressEvent ( QKeyEvent * event );    
    ///Mapping between variable and table index
    QMap<ModuleVariable*,int> m_variableMap;

};


#endif

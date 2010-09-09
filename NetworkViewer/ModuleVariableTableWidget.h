/**
    Copyright (C) 2009-2010 IntRoLab
    http://introlab.gel.usherbrooke.ca
    Dominic Letourneau, ing. M.Sc.A.
    Dominic.Letourneau@USherbrooke.ca
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

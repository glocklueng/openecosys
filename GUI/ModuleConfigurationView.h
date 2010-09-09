/**
     Copyright (C) 2009-2010 IntRoLab
     http://introlab.gel.usherbrooke.ca
     Dominic Letourneau, ing. M.Sc.A.
     Dominic.Letourneau@USherbrooke.ca
 */


#ifndef _MODULE_CONFIGURATION_VIEW_H_
#define _MODULE_CONFIGURATION_VIEW_H_

#include "ui_ModuleConfigurationView.h"
#include "NetworkModule.h"
#include <QScriptEngine>
#include <QScriptEngineAgent>
#include <QTimer>
#include <QTime>


/**
    The module configuration view is responsible of displaying the \ref ModuleConfiguration information
    It will be inserted in a MDI window.
*/
class ModuleConfigurationView : public QWidget, public Ui::ModuleConfigurationView
{
	
	Q_OBJECT;
	
public:
        /**
            Constructor
            \param module The network module containing the module information
        */
	ModuleConfigurationView(NetworkModule *module);
	
public slots:

        /**
            Update the view
        */
	void updateConfiguration();

signals:

        /**
            This signal will be emitted when the user clicks on a variable
            \param variable The variable requested
        */
	void scopeRequest(ModuleVariable *variable);


        /**
            This signal will be emitted when the user closes the view
        */
	void closeRequest();

protected slots:

        /**
            Called when the associated NetworkModule is destroyed.
        */
	void moduleDestroyed();

        /**
            Called when a cell is changed in the table view
        */
        void cellChanged ( int row, int column );

        /**
            Called when a variable is to be updated
            \param variable The updated variable
        */
        void variableUpdated(ModuleVariable *variable);

        /**
            A new variable has been added
            \param variable The newly added variable
        */
        void variableAdded(ModuleVariable *variable);

        /**
            Called when a cell is double clicked
            \param row the row
            \param column the colum
        */
        void cellDoubleClicked(int row, int column);

        void activateAllClicked();
        void deactivateAllClicked();

private:
	
        /**
            Called when the view needs to be updated
        */
	void updateView();	

        ///The associated network module
	NetworkModule *m_module;
};



#endif


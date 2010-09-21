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


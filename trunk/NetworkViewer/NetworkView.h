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

#ifndef _NETWORK_VIEW_H_
#define _NETWORK_VIEW_H_

#include "ui_NetworkView.h"
#include <QMainWindow>
#include "NetworkModule.h"
#include "ScopeView.h"
#include <QGraphicsPixmapItem>
#include <QBuffer>
#include <QMap>
#include "ModuleConfigurationView.h"
#include <QMdiSubWindow>
#include <QPushButton>
#include "NetworkModuleItem.h"
#include "CANInterfaceHandler.h"
#include "CANMessageEvent.h"
#include "NetworkScheduler.h"
#include "BasePlugin.h"
#include "DeviceSelectorDialog.h"
#include <QLabel>
#include <QLCDNumber>

/**
    Handler for menu trigger for plugins.
    \author Dominic Letourneau
*/
class PluginActionHandler : public QObject
{
	Q_OBJECT;

public:

        /**
            Constructor
            \param action the menu action
            \param name The plugin name
            \param parent the parent, default = NULL
        */
	PluginActionHandler(QAction* action, QString name, QObject *parent = NULL)
		: QObject(parent), m_action(action), m_name(name)
	{
		connect(m_action,SIGNAL(triggered(bool)),this,SLOT(actionTriggered(bool)));
	}

public slots:

        /**
            Called when the action menu is triggered
            \param checked TRUE when menu is checked
        */
	void actionTriggered(bool checked)
	{
		qDebug() << "PluginActionHandler for : " << m_name;
		emit pluginActivated(m_name);
	}

	signals:

        /**
            Signal emitted when the plugin is activated
            \param name The plugin name
        */
	void pluginActivated(const QString &name);

protected:

        ///The menu action
	QAction *m_action;
        ///The plugin name
	QString m_name;
};

/**
    The main window
    \author Dominic Letourneau
*/
class NetworkView : public QMainWindow, public Ui::NetworkView, public CANMessageObserverIF
{
        //The NetworkScheduler can access internal members
        friend class NetworkScheduler;

	Q_OBJECT;

public:
	

        /**
            Constructor
            \param parent Parent widget, default = NULL
        */
	NetworkView(QWidget *parent = NULL);


        ///Destructor
	virtual ~NetworkView();

        /**
            Create the scope view if it does not exist.
        */
	void createScopeView();	

        /**
            Add a module with a default configuration.
            \param config the configuration to use
        */
	void addModule(const ModuleConfiguration &config);

        /**
            \return the actual ScopeView. NULL if none.
        */
	ScopeView* getScopeView();

        /**
            Message notification, will be called from recvThread of \ref CANInterfaceHandler
            \param msg the CAN message
        */
	virtual void notifyCANMessage(const LABORIUS_MESSAGE &msg);

        /**
            Get all network modules
            \return QList<NetworkModule*> The sorted by deviceID list
        */
	QList<NetworkModule*> getModules();

        /**
            Request a variable update on the bus
            \param variable the required variable
        */
	void requestVariable(ModuleVariable *variable);

        /**
            Help Window request will popup a Web  browser with the determined url
            \param url The required URL to display
        */
        void helpWindowRequest(QString url);

        /**
            Get the module according to its DeviceID
            \param module_id the deviceID
            \return NetworkModule* The module, NULL if not found.
        */
        NetworkModule* getModule(int module_id);

        /**
            Get the module at index
            \param index the index
            \return NetworkModule* the module, NULL if not found
        */
        NetworkModule* getModuleAtIndex(int index);

        /**
            Create a subWindow for the MDI and add it to the Widow menu.
            \param title the title of the Window, default="Untitiled"
            \return QMdiSubWindow* The newly created sub window
        */
        QMdiSubWindow* createSubWindow(QString title = "Untitled");

public slots:

        /**
            Tile MDI Windows
        */
        void tileMDIWindows();

        /**
            Cascade MDI Windows
        */
        void cascadeMDIWindows();

        /**
            Close all MDI windows
        */
        void closeAllMDIWindows();

        /**
            Clear the text edit used for debug messages
        */
        void clearTextEdit();


        /**
            Save the content of the debug messages
        */
        void saveTextEdit();

        /**
            Request for the scope for a determined variable
            \param variable The variable to display
        */
        void scopeRequest(ModuleVariable *variable);

        /**
            Write a variable to the CAN bus
            \param variable The variable to write
        */
        void variableWrite(ModuleVariable *variable);

        /**
            Remove a module from the view. Will also delete the associated \ref NetworkModule
            \param module The item to remove
        */
        void removeModule(NetworkModuleItem* module);

        /**
            Print a debug message in the console
            \param message The debug message
        */
        void printDebug(const QString &message);

signals:

        /**
            Emit this signal when a module is added
        */
        void moduleAdded(NetworkModule* module);

        /**
            Emit this signal when a module is removed
        */
        void moduleRemoved(NetworkModule* module);


protected slots:
	


        /**
            The scope view has been destroyed
            /param object The object destroyed
        */
	void scopeDestroyed(QObject *object);

        /**
            When a module icon is double clicked
            \module the module item (icon)
        */
	void moduleDoubleClicked(NetworkModuleItem* module);

        /**
            Send alive request on the bus. This is called periodically
        */
	void sendAliveRequest();

        /**
            Activate a plugin from the menu
            \param name The plugin name
        */
	void pluginActivated(const QString& name);

        /**
            Device selection in the menu
            \param checked if the action is checked
        */
	void deviceSelectorTriggered(bool checked);


        /**
            Update connection statistics
        */
        void updateConnStats();

protected:

        /**
            Sort the module items (icons) by deviceID
        */
	void sortModuleItems();

        /**
            Select a module (bounding rectangle) on the screen
            \param module The module to select
        */
	void selectModule(NetworkModuleItem *module);

        /**
            Event Handler, will process \ref CANMessageEvent
            \param e The event to handle.
        */
	bool event ( QEvent * e );

        /**
            Process a CAN message
            \param msg The message to process
        */
        void processCANMessage(const LABORIUS_MESSAGE &msg);

        /**
            Write a variable to the CAN bus
            \param variable The variable to write
        */
	void writeVariable(ModuleVariable *variable);

        /**
            Create the plugin menu according to loaded plugins
        */
	void createPluginMenu();





        ///Mapping between module items (icons) and real modules
	QMap<NetworkModuleItem*, NetworkModule *> m_modules;

        ///The graphics scene to draw module icons (items)
	QGraphicsScene* m_scene;

        ///The scope view
	ScopeView *m_scopeView;

        ///The CAN interface handler
	CANInterfaceHandler *m_canHandler;

        ///The variable scheduling algorithm
	NetworkScheduler* m_scheduler;

        ///The label in the status bar (with the device selected name)
	QLabel *m_label;

        ///LCD number in the status bar for received messages
	QLCDNumber *m_receivedLCD;

        ///LCD number in the status bar for number of sent messages
        QLCDNumber *m_sentLCD;

        ///Conn stats timer
        QTimer *m_connStatsTimer;

};


#endif

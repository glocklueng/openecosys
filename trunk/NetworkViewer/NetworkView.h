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

#include <QMainWindow>
#include <QGraphicsPixmapItem>
#include <QBuffer>
#include <QMap>
#include <QMdiSubWindow>
#include <QPushButton>
#include <QLabel>
#include <QLCDNumber>

#include "ui_NetworkView.h"
#include "NetworkModule.h"
#include "ModuleConfigurationView.h"
#include "NetworkModuleItem.h"
#include "NETVInterfaceManager.h"
#include "BasePlugin.h"
#include "DeviceSelectorDialog.h"




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
class NetworkView : public QMainWindow, public Ui::NetworkView
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
            Get all network modules
            \return QList<NetworkModule*> The sorted by deviceID list
        */
        QList<NetworkModule*> getModules();



        /**
            \return a list of NETVInterfaceManagers
        */
        QList<NETVInterfaceManager*> getInterfaceManagerList();


        bool addNETVInterfaceManager(NETVInterfaceManager *manager);


        bool removeNETVInterfaceManager(NETVInterfaceManager *manager);


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

        /**
            Will create a plugin window
            \param pluginName the plugin name
            \param windowTitle The Window title
        */
        BasePlugin* createCustomPluginWindow(const QString &pluginName, const QString &windowTitle="");

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
            Will enable all variables from every module.
         */
        void enableAllModuleVariables();

        /**
            Will disable all variables from every module
         */
        void disableAllModuleVariables();


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
            Remove a module from the view. Will also delete the associated \ref NetworkModule
            \param module The item to remove
        */
        void removeModule(NetworkModuleItem* module);

        /**
            Print a debug message in the console
            \param message The debug message
        */
        void printDebug(const QString &message);

        /**
            Add a module (detected on the bus or pseudo module)
        */
        void addModule(NetworkModule *module);

        /**
            Remove a Module
        */
        void removeModule(NetworkModule* module);

signals:

        void moduleRemoved(NetworkModule *module);
        void moduleAdded(NetworkModule *module);

protected slots:

        /**
            Called when the dock widget is changed
        */
        void moduleDockWidgetLocationChanged(Qt::DockWidgetArea area );

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
            Preferences  in the menu
            \param checked if the action is checked
        */
        void preferencesTriggered(bool checked);

        /**
            About in the menu
            \param checked if the action is checked
        */
        void aboutTriggered(bool checked);

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
            Create the plugin menu according to loaded plugins
        */
	void createPluginMenu();

        ///Mapping between module items (icons) and real modules
	QMap<NetworkModuleItem*, NetworkModule *> m_modules;

        ///The graphics scene to draw module icons (items)
	QGraphicsScene* m_scene;

        ///The scope view
        BasePlugin *m_scopeView;

        ///Interface manager
        ///TODO Add multiple interface managers
        QList<NETVInterfaceManager*> m_interfaceManagerList;

        ///The label in the status bar (with the device selected name)
	QLabel *m_label;

        ///LCD number in the status bar for received messages
	QLCDNumber *m_receivedLCD;

        ///LCD number in the status bar for number of sent messages
        QLCDNumber *m_sentLCD;

        ///Conn stats timer
        QTimer *m_connStatsTimer;

        ///Where is docked the modules
        Qt::DockWidgetArea m_moduleDockWidgetArea;

};


#endif

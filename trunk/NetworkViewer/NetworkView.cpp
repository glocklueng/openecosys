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

#include "NetworkView.h"
#include <QCoreApplication>
#include "CAN_define.h"
#include <QTime>
#include "BasePlugin.h"
#include <iostream>
#include <QWebView>
#include <QFileDialog>
#include "PreferencesDialog.h"
#include "ScopeView.h"

bool ModuleGreater(NetworkModule* first, NetworkModule* second)
{
    if (first->getConfiguration()->getDeviceID() > second->getConfiguration()->getDeviceID())
    {
        return false;
    }
    else
    {
        return true;
    }
}


NetworkView::NetworkView(QWidget *parent)
    :	QMainWindow(parent), m_scopeView(NULL), m_canHandler(NULL), m_scheduler(NULL)
{
    setupUi(this);

    //Create scene
    m_scene = new QGraphicsScene(this);

    //Set scene
    m_moduleGraphicsView->setScene(m_scene);

    //Setup MDI
    m_mdiArea->setDocumentMode(true);

    //Add scope
    createScopeView();

    m_label = new QLabel("NOT CONNECTED",this);
    statusbar->addPermanentWidget(m_label);

    m_receivedLCD = new QLCDNumber(10,this);
    m_receivedLCD->display(0);
    m_sentLCD = new QLCDNumber(10,this);
    m_sentLCD->display(0);

    statusbar->addPermanentWidget(new QLabel("Received:",this));
    statusbar->addPermanentWidget(m_receivedLCD);
    statusbar->addPermanentWidget(new QLabel("Sent:",this));
    statusbar->addPermanentWidget(m_sentLCD);
    statusbar->showMessage("Please select device first from the File menu");

    //Create a network scheduler
    m_scheduler = new NetworkScheduler(this);

    //Create the plugin menu (they are scanned in the main.cpp file)
    createPluginMenu();

    //Menu actions
    connect(actionDevice_Selection,SIGNAL(triggered(bool)),this,SLOT(deviceSelectorTriggered(bool)));
    connect(actionPreferences,SIGNAL(triggered(bool)),this,SLOT(preferencesTriggered(bool)));


    connect(actionTile_Windows,SIGNAL(triggered()),this,SLOT(tileMDIWindows()));
    connect(actionClose_All_Windows,SIGNAL(triggered()),this,SLOT(closeAllMDIWindows()));
    connect(actionCascade_Windows,SIGNAL(triggered()),this,SLOT(cascadeMDIWindows()));
    connect(actionQuit,SIGNAL(triggered()),this,SLOT(close()));

    //Show Connect to device dialog
    deviceSelectorTriggered(true);

    //Conn statistics timer
    m_connStatsTimer = new QTimer(this);
    connect(m_connStatsTimer,SIGNAL(timeout()),this,SLOT(updateConnStats()));
    m_connStatsTimer->start(100);//100ms timer (10Hz)


    //Debug area
    connect(clearToolButton,SIGNAL(clicked()),this,SLOT(clearTextEdit()));
    connect(saveToolButton,SIGNAL(clicked()),this,SLOT(saveTextEdit()));
}


NetworkView::~NetworkView()
{
    qDebug("NetworkView::~NetworkView()");

    if (m_canHandler)
    {
        m_canHandler->unregisterObserver(this);

        qDebug("deleting m_canHandler");
        delete m_canHandler;
        m_canHandler = NULL;
    }

}


void NetworkView::notifyCANMessage(const LABORIUS_MESSAGE &msg)
{
    //qDebug("NetworkView::notifyCANMessage(const LABORIUS_MESSAGE &msg)");
    
    //Posting message to self (in the GUI thread)
    CANMessageEvent *event = new CANMessageEvent(msg);
    QCoreApplication::postEvent (this, event, Qt::HighEventPriority);
}

bool NetworkView::event ( QEvent * e )
{
    if (e->type() == QEvent::User)
    {
        //qDebug("NetworkView::event %p QEvent::User",e);
        if (CANMessageEvent *event = dynamic_cast<CANMessageEvent*>(e))
        {


            processCANMessage(event->getMessage());
            return true;
        }
    }

    return QMainWindow::event(e);
}

void NetworkView::processCANMessage(const LABORIUS_MESSAGE &msg)
{
    //qDebug("NetworkView::processCANMessage(const LABORIUS_MESSAGE &msg)");
    //CANDevice::printMessage(msg,std::cout);

    //WILL HANDLE ONLY REQUEST TYPE
    if (msg.msg_type == CAN_TYPE_REQUEST_DATA)
    {
    	
        //CANDevice::printMessage(msg,std::cout);

        //Let's see to which module it belongs...
        //Update the variable if it fits...
        for(QMap<NetworkModuleItem*, NetworkModule *>::iterator iter = m_modules.begin(); iter != m_modules.end(); iter++)
        {
            NetworkModule *module = iter.value();
            
            ModuleConfiguration *conf = module->getConfiguration();
            Q_ASSERT(conf);

            if (conf->getDeviceID() == msg.msg_dest)
            {
                for(int i =0; i < conf->size(); i++)
                {
                    //Need to check the offset and the memory type
                    if ((*conf)[i]->getOffset() == msg.msg_cmd && (*conf)[i]->getMemType() == (ModuleVariable::VARIABLE_MEMORY_TYPE) (msg.msg_boot >> 1))
                    {
                        //FOUND IT
                        //UPDATING VALUE
                    	//qDebug("found variable : ");
                        (*conf)[i]->setValue(msg.msg_data, msg.msg_data_length);
                        break;
                    }
                }
            }
        } 
    }//type == REQUEST_DATA
    else if (msg.msg_type == CAN_TYPE_EVENTS)
    {

        if (msg.msg_cmd == CAN_EVENTS_CMD_ALIVE && msg.msg_data_length == 8)
        {
            //qDebug("ALIVE REQUEST ANSWERED...");

            /*
				msg.msg_data[0] = config->module_state;
				msg.msg_data[1] = config->project_id;
				msg.msg_data[2] = config->module_id;
				msg.msg_data[3] = config->code_version;
				msg.msg_data[4] = config->table_version;
				msg.msg_data[5] = config->boot_delay;
				msg.msg_data[6] = config->devid_low;
				msg.msg_data[7] = config->devid_high;
			*/
            int module_state = msg.msg_data[0];
            int project_id = msg.msg_data[1];
            int module_id = msg.msg_data[2];
            int code_version = msg.msg_data[3];
            int table_version = msg.msg_data[4];
            int boot_delay = msg.msg_data[5];
            int devID = (((int) msg.msg_data[7]) << 8) | (int) msg.msg_data[6];

            //Look for already existing modules...
            bool found = false;
            for (QMap<NetworkModuleItem*, NetworkModule*>::iterator i = m_modules.begin(); i != m_modules.end(); i++)
            {
                NetworkModule *module = i.value();

                if (module->getConfiguration()->getDeviceID() == module_id)
                {
                    found = true;
                    //qDebug("already found module : %i",module_id);
                    i.key()->timeUpdate(QTime::currentTime());
                }
            }

            if (!found)
            {
                qDebug("Inserting new module %i",module_id);

                //fill configuration information...
                ModuleConfiguration conf(project_id, code_version, devID, module_state, table_version, module_id);

                addModule(conf);
            }

        }
    }//type == EVENTS
}

void NetworkView::createScopeView()
{
    m_scopeView = new ScopeView(this);

    //Create MDI window
    QMdiSubWindow *subWindow = createSubWindow("Scope");
    subWindow->setWidget(m_scopeView);
    subWindow->setAttribute(Qt::WA_DeleteOnClose);
    m_mdiArea->addSubWindow(subWindow);
    subWindow->resize(640,480);
    subWindow->show();


    connect(m_scopeView,SIGNAL(destroyed(QObject*)),this,SLOT(scopeDestroyed(QObject*)));

}

void NetworkView::addModule(const ModuleConfiguration &config)
{
    NetworkModule *module = new NetworkModule(config,this);


    qDebug("NetworkView::addModule created module %p",module);

    //Create view for this module
    int index = m_modules.size();

    NetworkModuleItem* item = new NetworkModuleItem(module);

    qDebug("NetworkView::addModule created module %p, item = %p",module,item);

    item->timeUpdate(QTime::currentTime());

    QRectF rect = item->boundingRect();

    item->setPos(index * (rect.width() * 0.45), 0);

    item->show();

    m_scene->addItem(item);

    m_modules.insert(item,module);


    connect(item,SIGNAL(doubleClicked(NetworkModuleItem*)),this,SLOT(moduleDoubleClicked(NetworkModuleItem*)));
    connect(item,SIGNAL(removeModule(NetworkModuleItem*)),this,SLOT(removeModule(NetworkModuleItem*)));

    //User modification
    connect(module,SIGNAL(variableWrite(ModuleVariable*)),this,SLOT(variableWrite(ModuleVariable*)));

    //Add module to the scheduler
    m_scheduler->addModule(module);


    //Sort all modules
    sortModuleItems();

    //Emit signal
    emit moduleAdded(module);
}

void NetworkView::sortModuleItems()
{
    //Will get a sorted by module_id list
    QList<NetworkModule*> allModules = getModules();

    //This is not very efficient, but will work
    for (int index = 0; index < allModules.size(); index++)
    {
        //Let's find the item associated with the module
        for (QMap<NetworkModuleItem*, NetworkModule *>::iterator iter = m_modules.begin(); iter != m_modules.end(); iter++)
        {
            if (iter.value() == allModules[index])
            {
                //Found it, placing it at the right index
                NetworkModuleItem *item = iter.key();
                QRectF rect = item->boundingRect();
                item->setPos(index * (rect.width() * 0.25), 0);
                break;
            }
        }
    }
}

void NetworkView::moduleDoubleClicked(NetworkModuleItem* module)
{
    qDebug("RECV SLOT  : moduleDoubleClicked %p",module);
    //selectModule(module);

    NetworkModule *netModule = module->getNetworkModule();

    if (netModule)
    {
        qDebug("RECV SLOT  : moduleDoubleClicked netModule %p",netModule);
        ModuleConfigurationView *view = new ModuleConfigurationView(this,netModule);

        view->show();

        //Create MDI window
        QMdiSubWindow *subWindow = createSubWindow("Configuration Module :" + QString::number(netModule->getConfiguration()->getDeviceID()));
        subWindow->setObjectName(QString("Module : ") + QString::number(netModule->getConfiguration()->getDeviceID()));
        subWindow->setWindowTitle(QString("Module : ") + QString::number(netModule->getConfiguration()->getDeviceID()));
        subWindow->setWidget(view);
        subWindow->setAttribute(Qt::WA_DeleteOnClose);
        m_mdiArea->addSubWindow(subWindow);
        subWindow->resize(640,480);
        subWindow->show();

        //closing when requested...
        connect(view,SIGNAL(closeRequest()),subWindow,SLOT(close()));


        //Connect scope request
        connect(view,SIGNAL(scopeRequest(ModuleVariable*)),this,SLOT(scopeRequest(ModuleVariable*)));
    }
    qDebug("RECV SLOT  (done): moduleDoubleClicked %p",module);
}

void NetworkView::variableWrite(ModuleVariable *variable)
{
    //qDebug("------------------------------------ NetworkView::variableWrite(const ModuleVariable &variable)");
    writeVariable(variable);
}

void NetworkView::scopeRequest(ModuleVariable *variable)
{
    qDebug("NetworkView::scopeRequest(const ModuleVariable &variable)");

    if (!m_scopeView)
    {
        createScopeView();
    }

    m_scopeView->addCurve(variable);
}

ScopeView* NetworkView::getScopeView()
{
    return m_scopeView;
}

void NetworkView::scopeDestroyed(QObject *object)
{
    qDebug("scopeDestroyed");
    m_scopeView = NULL;
}

void NetworkView::writeVariable(ModuleVariable *variable)
{
    Q_ASSERT(variable);

    if (variable->getMemType() < ModuleVariable::SCRIPT_VARIABLE && variable->getOffset() >= 0)
    {

	//qDebug("NetworkView::writeVariable %s",variable->getName().toStdString().c_str());

	//Building CAN request...
	LABORIUS_MESSAGE canMsg;

	canMsg.msg_priority = CAN_PRIORITY_HIGHEST;
	canMsg.msg_type = CAN_TYPE_REQUEST_DATA;
	
	//Requesting the right type of memory
	switch(variable->getMemType())
	{
        case ModuleVariable::RAM_VARIABLE:
            canMsg.msg_boot = (CAN_REQUEST_RAM << 1) | (CAN_REQUEST_WRITE);
            break;

        case ModuleVariable::EEPROM_VARIABLE:
            canMsg.msg_boot = (CAN_REQUEST_EEPROM << 1) | (CAN_REQUEST_WRITE);
            break;
	}
	
	canMsg.msg_cmd = variable->getOffset();
	canMsg.msg_dest = variable->getDeviceID();

	//This is a NOT remote request
	canMsg.msg_remote = 0;

	//Of the size of the variable...
	canMsg.msg_data_length = variable->getLength();

	//Set The data
	QByteArray array = variable->toByteArray();

	//Copy the data
	for (int i =0; i < min(8,array.size()); i++)
	{
            canMsg.msg_data[i] = array[i];
	}

	//Sending to CAN bus...
	if (m_canHandler)
	{				
            m_canHandler->pushCANMessage(canMsg);
	}
    }
}

void NetworkView::requestVariable(ModuleVariable *variable)
{
    Q_ASSERT(variable);

    NetworkModule *module = this->getModule(variable->getDeviceID());
    Q_ASSERT(module);



    if (module->active() &&   variable->getMemType() < ModuleVariable::SCRIPT_VARIABLE && variable->getOffset() >= 0 )
    {
        //qDebug("NetworkView::requestVariable %s",variable.getName().toStdString().c_str());




        //Building CAN request...
        LABORIUS_MESSAGE canMsg;

        canMsg.msg_priority = CAN_PRIORITY_HIGHEST;
        canMsg.msg_type = CAN_TYPE_REQUEST_DATA;

        //Requesting the right type of memory
        switch(variable->getMemType())
        {
        case ModuleVariable::RAM_VARIABLE:
            canMsg.msg_boot = (CAN_REQUEST_RAM << 1) | (CAN_REQUEST_READ);
            break;

        case ModuleVariable::EEPROM_VARIABLE:
            canMsg.msg_boot = (CAN_REQUEST_EEPROM << 1) | (CAN_REQUEST_READ);
            break;

        }

        canMsg.msg_cmd = variable->getOffset();
        canMsg.msg_dest = variable->getDeviceID();

        //This is a remote request
        canMsg.msg_remote = 1;

        //Of the size of the variable...
        canMsg.msg_data_length = variable->getLength();

        //Sending to CAN bus...
        if (m_canHandler)
        {
            m_canHandler->pushCANMessage(canMsg);
        }
    }
}

void NetworkView::printDebug(const QString &message)
{
    QTime time = QTime::currentTime();
    m_textEdit->append(time.toString("<b>[hh:mm:ss.zz]</b> ") + message);
}

void NetworkView::sendAliveRequest()
{

    //Look for too old modules
    //Remove modules that are not present anymore on the  bus
    for (QMap<NetworkModuleItem*, NetworkModule*>::iterator i = m_modules.begin(); i != m_modules.end(); i++)
    {
        NetworkModuleItem *module = i.key();

        QTime lastUpdate = module->getLastUpdateTime();

        //MORE THAN 5 SECONDS INACTIVE?
        if (lastUpdate.addSecs(5) < QTime::currentTime())
        {
            i.key()->setActive(false);
            //This is really important because of the removeModule will alter the QMap structure...
            //removeModule(i.key());
            //break;
        }
        else
        {
            i.key()->setActive(true);
        }
    }

    //Building CAN request...
    LABORIUS_MESSAGE canMsg;

    canMsg.msg_priority = CAN_PRIORITY_HIGHEST;
    canMsg.msg_type = CAN_TYPE_EVENTS;
    canMsg.msg_boot = 0;
    canMsg.msg_cmd = CAN_EVENTS_CMD_ALIVE;

    //broadcast
    canMsg.msg_dest = 0xFF;

    //This is a remote request
    canMsg.msg_remote = 1;

    //Of zero size...
    canMsg.msg_data_length = 8;

    //Sending to CAN bus...
    if (m_canHandler)
    {
        m_canHandler->pushCANMessage(canMsg);
    }
}

void NetworkView::removeModule(NetworkModuleItem* module)
{
    //qDebug("NetworkView::removeModule(NetworkModuleItem* module = %p)",module);
    NetworkModule *realModule = m_modules[module];

    //qDebug("size before : %i",m_modules.size() );

    //Remove from scheduler
    m_scheduler->removeModule(realModule);

    //Remove from Map
    m_modules.remove(module);

    //Remove Item
    m_scene->removeItem(module);
    delete module;

    //qDebug("size after : %i",m_modules.size() );

    //Remove network module
    if (realModule)
    {
        emit moduleRemoved(realModule);
        delete realModule;
    }

    //refresh module list view
    sortModuleItems();
}

void NetworkView::createPluginMenu()
{
    QMap<QString, BasePlugin::BasePluginFactory*> & plugins = BasePlugin::loadedPlugins();



    for (QMap<QString, BasePlugin::BasePluginFactory*>::iterator iter = plugins.begin();
    iter != plugins.end(); iter++)
    {
        QAction *action = menuPlugin->addAction(iter.key());


        PluginActionHandler *handler = new PluginActionHandler(action,iter.key(),this);

        connect(handler, SIGNAL(pluginActivated(const QString&)),this,SLOT(pluginActivated(const QString&)));
    }

}

void NetworkView::pluginActivated(const QString& name)
{
    qDebug() << "NetworkView::pluginActivated : " << name;
    QMap<QString, BasePlugin::BasePluginFactory*> & plugins = BasePlugin::loadedPlugins();

    BasePlugin::BasePluginFactory* factory = plugins[name];

    if (factory)
    {
        BasePlugin *plugin = factory->create(this);

        //initialize plugin
        plugin->init();

        //Create MDI window
        QMdiSubWindow *subWindow = createSubWindow("Plugin : " + name);
        subWindow->setWidget(plugin);
        m_mdiArea->addSubWindow(subWindow);
        subWindow->show();
    }
}

QList<NetworkModule*> NetworkView::getModules()
{
    QList<NetworkModule*> allModules;

    for (QMap<NetworkModuleItem*, NetworkModule *>::iterator iter = m_modules.begin(); iter != m_modules.end(); iter++)
    {
        allModules.push_back(iter.value());
    }

    //Sort modules by device_id...
    qSort(allModules.begin(),allModules.end(),ModuleGreater);

    return allModules;

}

void NetworkView::preferencesTriggered(bool checked)
{
    PreferencesDialog prefs(this);
    prefs.exec();
}

NetworkScheduler* NetworkView::getNetworkScheduler()
{
    return m_scheduler;
}

void NetworkView::deviceSelectorTriggered(bool checked)
{
    DeviceSelectorDialog dialog(this);


    qDebug("will start dialog");


    ///Execute dialog
    int return_value = dialog.exec();

    qDebug("done exec");

    QStringList deviceList = CANDevice::deviceList();

    qDebug("Device list:");

    for (int i= 0; i < deviceList.size(); i++)
    {
        qDebug() << deviceList[i];
    }


    if (return_value)
    {
        //Destroy old handler if required...
        if(m_canHandler)
        {
            qDebug("Destroying old handler");
            m_canHandler->unregisterObserver(this);
            delete m_canHandler;
            m_canHandler = NULL;
        }

        //Update selected device
        m_label->setText(dialog.selectedDevice());

        if (dialog.getFactory())
        {

            CANDevice *dev = NULL;

            qDebug("Found factory");
	    if(dialog.args().size())
	    {
            	dev = CANDevice::createDevice(dialog.selectedDevice(), dialog.args().toStdString().c_str());
	    }
	    else
            {
		dev = CANDevice::createDevice(dialog.selectedDevice(),NULL);
	    }

            if (dev)
            {
                m_canHandler = new CANInterfaceHandler(dev,NULL,this);
                m_canHandler->registerObserver(this);

            }

        }
    } //if return value

    qDebug("device selection returned");
}



NetworkModule* NetworkView::getModule(int module_id)
{
    NetworkModule *mod = NULL;

    for (QMap<NetworkModuleItem*, NetworkModule *>::iterator iter = m_modules.begin(); iter != m_modules.end(); iter++)
    {
        if (iter.value()->getConfiguration()->getDeviceID() == module_id)
        {
            mod = iter.value();
            break;
        }
    }

    return mod;
}

void NetworkView::helpWindowRequest(QString url)
{
    //Create MDI window
    QMdiSubWindow *subWindow = createSubWindow(url);
    QWebView *webview = new QWebView(this);
    webview->load(QUrl(url));
    subWindow->setWidget(webview);
    m_mdiArea->addSubWindow(subWindow);
    subWindow->resize(1024,768);
    subWindow->show();
    subWindow->raise();
}

QMdiSubWindow* NetworkView::createSubWindow(QString title)
{
    //Create MDI window
    QMdiSubWindow *subWindow = new QMdiSubWindow(this);
    subWindow->setWindowTitle(title);

    subWindow->setAttribute(Qt::WA_DeleteOnClose);

    //Create Window menu
    QAction *action = menuWindow->addAction(title);

    //This will cause the window to show its normal state and raise when the menu is clicked
    connect(action,SIGNAL(triggered()),subWindow,SLOT(showNormal()));
    connect(action,SIGNAL(triggered()),subWindow,SLOT(raise()));

    //This will cause the menu to be removed with the window is deleted
    connect(subWindow,SIGNAL(destroyed()),action,SLOT(deleteLater()));

    //Returning newly created window
    return subWindow;
}

void NetworkView::tileMDIWindows()
{
    m_mdiArea->tileSubWindows();
}

void NetworkView::cascadeMDIWindows()
{
    m_mdiArea->cascadeSubWindows();
}

void NetworkView::closeAllMDIWindows()
{
    m_mdiArea->closeAllSubWindows();
}


void NetworkView::updateConnStats()
{
    if (m_canHandler)
    {
        m_receivedLCD->display(m_canHandler->messageReceivedCounter());
        m_sentLCD->display(m_canHandler->messageSentCounter());
    }
}

void NetworkView::clearTextEdit()
{
    m_textEdit->clear();
}

void NetworkView::saveTextEdit()
{

    QString fileName = QFileDialog::getSaveFileName(this,
         tr("Save Debug"), ".", tr("Text Files (*.txt)"));

    if (fileName.size())
    {
        QFile fileOutput(fileName);
        fileOutput.open(QFile::WriteOnly);
        QTextStream stream(&fileOutput);
        stream << m_textEdit->toPlainText();
        fileOutput.close();
    }

}

NetworkModule* NetworkView::getModuleAtIndex(int index)
{
    if (index < m_modules.size())
    {
        return getModules()[index];
    }
    return NULL;
}

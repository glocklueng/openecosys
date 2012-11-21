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
#include "NETV_define.h"
#include <QTime>
#include "BasePlugin.h"
#include <iostream>
#include <QWebView>
#include <QFileDialog>
#include "PreferencesDialog.h"
#include "BasePluginEvent.h"
#include <QMenu>
#include "NETVInterfaceManagerView.h"

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
    :	QMainWindow(parent), m_scopeView(NULL), m_moduleDockWidgetArea(Qt::TopDockWidgetArea)
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

    m_label = new QLabel("0",this);
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

    //Create the plugin menu (they are scanned in the main.cpp file)
    createPluginMenu();

    //Menu actions
    connect(actionDevice_Selection,SIGNAL(triggered(bool)),this,SLOT(deviceSelectorTriggered(bool)));
    connect(actionPreferences,SIGNAL(triggered(bool)),this,SLOT(preferencesTriggered(bool)));
    connect(actionAbout,SIGNAL(triggered(bool)),this,SLOT(aboutTriggered(bool)));

    //Tools
    connect(actionEnable_All_Variables,SIGNAL(triggered()),this,SLOT(enableAllModuleVariables()));
    connect(actionDisable_All_Variables,SIGNAL(triggered()),this,SLOT(disableAllModuleVariables()));

    //Windows
    connect(actionTile_Windows,SIGNAL(triggered()),this,SLOT(tileMDIWindows()));
    connect(actionClose_All_Windows,SIGNAL(triggered()),this,SLOT(closeAllMDIWindows()));
    connect(actionCascade_Windows,SIGNAL(triggered()),this,SLOT(cascadeMDIWindows()));
    connect(actionQuit,SIGNAL(triggered()),this,SLOT(close()));

    //Show Connect to device dialog
    NETVInterfaceManagerView *managerView = new NETVInterfaceManagerView(this,true);
    //managerView->show();

    //Conn statistics timer
    m_connStatsTimer = new QTimer(this);
    connect(m_connStatsTimer,SIGNAL(timeout()),this,SLOT(updateConnStats()));
    m_connStatsTimer->start(100);//100ms timer (10Hz)


    //Debug area
    connect(clearToolButton,SIGNAL(clicked()),this,SLOT(clearTextEdit()));
    connect(saveToolButton,SIGNAL(clicked()),this,SLOT(saveTextEdit()));


    //Dock widget change
    connect(m_moduleDockWidget,SIGNAL(dockLocationChanged(Qt::DockWidgetArea)),this,SLOT(moduleDockWidgetLocationChanged(Qt::DockWidgetArea)));

    //testing broadcaster...
    m_broadcaster = new NETVBroadcaster(8888,12345,this);

}


NetworkView::~NetworkView()
{
    qDebug("NetworkView::~NetworkView()");

    while(m_interfaceManagerList.size() > 0)
    {
        delete m_interfaceManagerList.first();
        m_interfaceManagerList.pop_front();
    }

}

void NetworkView::createScopeView()
{
    m_scopeView = createCustomPluginWindow("ScopeView","ScopeView (MAIN)");

    if (m_scopeView)
    {
        connect(m_scopeView,SIGNAL(destroyed(QObject*)),this,SLOT(scopeDestroyed(QObject*)));
    }
}

void NetworkView::addModule(NetworkModule *module)
{
    Q_ASSERT(module);
    int index = m_modules.size();
    NetworkModuleItem *item = new NetworkModuleItem(module);
    QRectF rect = item->boundingRect();
    item->setPos(index * (rect.width()), 0);
    item->show();
    m_scene->addItem(item);
    m_modules.insert(item,module);

    connect(item,SIGNAL(doubleClicked(NetworkModuleItem*)),this,SLOT(moduleDoubleClicked(NetworkModuleItem*)));
    connect(item,SIGNAL(removeModule(NetworkModuleItem*)),this,SLOT(removeModule(NetworkModuleItem*)));

    emit moduleAdded(module);

    //Sort all modules
    sortModuleItems();
}

void NetworkView::sortModuleItems()
{
    //Will get a sorted by module_id list
    QList<NetworkModule*> allModules = getModules();


    float cum_width = 0;
    float cum_height = 0;

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
		rect = rect.unite(item->childrenBoundingRect());


                if (m_moduleDockWidgetArea == Qt::LeftDockWidgetArea || m_moduleDockWidgetArea == Qt::RightDockWidgetArea)
                {
                    item->setPos(0, cum_height + (rect.height() / 2) * 0.35);
                }
                else
                {
                    item->setPos(cum_width + (rect.width() / 2) * 0.35, 0);
                }
		
		//cumulate and pad...
		cum_width += rect.width() * 0.35 + 10;
                cum_height += rect.height() * 0.35 + 10;
                break;
            }
        }
    }

    m_scene->setSceneRect(m_scene->itemsBoundingRect());
    //m_moduleGraphicsView->fitInView(m_scene->itemsBoundingRect(),Qt::KeepAspectRatio);
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

void NetworkView::scopeRequest(ModuleVariable *variable)
{
    qDebug("NetworkView::scopeRequest(const ModuleVariable &variable)");

    if (!m_scopeView)
    {
        createScopeView();
    }

    if (m_scopeView)
    {
        //Send Plugin message
        QCoreApplication::postEvent(m_scopeView,new BasePluginEvent("addCurve",QVariant((unsigned long long)(variable))));
    }
}

void NetworkView::scopeDestroyed(QObject *object)
{
    qDebug("scopeDestroyed");
    m_scopeView = NULL;
}


void NetworkView::printDebug(const QString &message)
{
    QTime time = QTime::currentTime();
    m_textEdit->append(time.toString("<b>[hh:mm:ss.zz]</b> ") + message);
}




void NetworkView::removeModule(NetworkModuleItem* module)
{
    //qDebug("NetworkView::removeModule(NetworkModuleItem* module = %p)",module);
    NetworkModule *realModule = m_modules[module];

    //Remove from Map
    m_modules.remove(module);

    //Remove Item
    m_scene->removeItem(module);
    delete module;

    //qDebug("size after : %i",m_modules.size() );

    //Remove network module
    if (realModule)
    {

        //is it a pseudo module if so, delete it
        if (realModule->getConfiguration()->getDeviceID() > 255)
        {
            emit moduleRemoved(realModule);
            delete realModule;
        }
        else
        {

            emit moduleRemoved(realModule);

            for (unsigned int i = 0; i < m_interfaceManagerList.size(); i++)
            {
                //Remove from manager
                m_interfaceManagerList[i]->removeModule(realModule);
            }
        }
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

BasePlugin* NetworkView::createCustomPluginWindow(const QString &pluginName, const QString &windowTitle)
{
    QMap<QString, BasePlugin::BasePluginFactory*> & plugins = BasePlugin::loadedPlugins();

    BasePlugin::BasePluginFactory* factory = plugins[pluginName];

    BasePlugin *plugin = NULL;

    if (factory)
    {
        plugin = factory->create(this);

        //initialize plugin
        plugin->init();

        //Create MDI window
        QMdiSubWindow *subWindow = createSubWindow(windowTitle);
        subWindow->setWidget(plugin);
        m_mdiArea->addSubWindow(subWindow);
        subWindow->show();
    }

    return plugin;
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



void NetworkView::deviceSelectorTriggered(bool checked)
{

    //Execute interface manager view
    NETVInterfaceManagerView managerView(this,false);

    managerView.exec();

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


void NetworkView::enableAllModuleVariables()
{
    for (QMap<NetworkModuleItem*, NetworkModule *>::iterator iter = m_modules.begin(); iter != m_modules.end(); iter++)
    {
        ModuleConfiguration *conf = iter.value()->getConfiguration();

        for (unsigned int i = 0; i < conf->size(); i++)
        {
            ModuleVariable *var = (*conf)[i];
            var->setActivated(true);
        }
    }

}


void NetworkView::disableAllModuleVariables()
{
    for (QMap<NetworkModuleItem*, NetworkModule *>::iterator iter = m_modules.begin(); iter != m_modules.end(); iter++)
    {
        ModuleConfiguration *conf = iter.value()->getConfiguration();

        for (unsigned int i = 0; i < conf->size(); i++)
        {
            ModuleVariable *var = (*conf)[i];
            var->setActivated(false);
        }
    }
}



void NetworkView::updateConnStats()
{

    //Will get stats of all managers compiled
    int sent = 0;
    int recv = 0;

    for (unsigned int i = 0; i < m_interfaceManagerList.size(); i++)
    {
        NETVInterfaceHandler* handler = m_interfaceManagerList[i]->getInterfaceHandler();
        sent += handler->messageSentCounter();
        recv += handler->messageReceivedCounter();
    }

    m_label->setText(QString::number(m_interfaceManagerList.size()));
    m_receivedLCD->display(recv);
    m_sentLCD->display(sent);
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


void NetworkView::moduleDockWidgetLocationChanged(Qt::DockWidgetArea area )
{
    qDebug("NetworkView::moduleDockWidgetLocationChanged(Qt::DockWidgetArea area = %i)",area);
    m_moduleDockWidgetArea = area;

    //Will display modules according to dock position
    sortModuleItems();
}

void NetworkView::aboutTriggered(bool checked)
{
    helpWindowRequest("http://sourceforge.net/apps/mediawiki/openecosys/index.php?title=NetworkViewer:About");
}

QList<NETVInterfaceManager*> NetworkView::getInterfaceManagerList()
{
    return m_interfaceManagerList;
}

bool NetworkView::addNETVInterfaceManager(NETVInterfaceManager *manager)
{
    if (!m_interfaceManagerList.contains(manager))
    {
        //Add to the list
        m_interfaceManagerList.push_back(manager);

        //Connect signals
        connect(manager,SIGNAL(moduleAdded(NetworkModule*)),this,SLOT(addModule(NetworkModule*)));
        connect(manager,SIGNAL(moduleRemoved(NetworkModule*)),this,SLOT(removeModule(NetworkModule*)));
        connect(manager,SIGNAL(moduleActive(NetworkModule*,bool)),this,SLOT(moduleActive(NetworkModule*,bool)));


        emit interfaceAdded(manager);


        return true;
    }

    return false;
}


bool NetworkView::removeNETVInterfaceManager(NETVInterfaceManager *manager)
{

    if (m_interfaceManagerList.contains(manager))
    {
        m_interfaceManagerList.removeAll(manager);

        emit interfaceRemoved(manager);

        delete manager;

        return true;
    }

    return false;
}

void NetworkView::removeModule(NetworkModule* module)
{
    //Reverse lookup on map
    //QMap<NetworkModuleItem*, NetworkModule *> m_modules;
    for (QMap<NetworkModuleItem*, NetworkModule *>::iterator iter = m_modules.begin(); iter != m_modules.end(); iter++)
    {
        if (iter.value() == module)
        {
            removeModule(iter.key());
            break;
        }
    }
}


void NetworkView::moduleActive(NetworkModule *module, bool active)
{
    for (QMap<NetworkModuleItem*, NetworkModule *>::iterator iter = m_modules.begin(); iter != m_modules.end(); iter++)
    {
        if (iter.value() == module)
        {
            iter.key()->setActive(active);
            break;
        }
    }
}



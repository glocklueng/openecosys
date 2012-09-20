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

#include "DeclarativeTest.h"
#include "NetworkView.h"
#include <QFileDialog>
#include <QDeclarativeContext>
#include <QDeclarativeEngine>

Q_DECLARE_METATYPE(QList<NetworkModule*>)

//This will insert the plugin in the dictionary...
static int declarative_test_plugin_init = BasePlugin::registerPlugin("DeclarativeTest",new BasePlugin::PluginFactory<DeclarativeTest>());



DeclarativeTest::DeclarativeTest(NetworkView *view)
    : BasePlugin(view)
{
    qDebug("DeclarativeTest::DeclarativeTest(NetworkView *view = %p)",view);

    //Setup UI
    m_ui.setupUi(this);

    //Will automatically size root object to view
    m_ui.m_declarativeView->setResizeMode(QDeclarativeView::SizeRootObjectToView);



    //Connect load button
    connect(m_ui.m_loadButton,SIGNAL(clicked()),this,SLOT(loadButtonClicked()));


    resize(800,600);
}

void DeclarativeTest::init()
{
    qmlRegisterType<QMLVariable>("NETVLibrary", 1, 0, "QMLVariable");
}

void DeclarativeTest::terminate()
{

}

void DeclarativeTest::loadButtonClicked()
{
    qDebug("DeclarativeTest::loadButtonClicked()");


    QString fileName = QFileDialog::getOpenFileName(NULL, "Open QML Script File",
                                                    m_lastPath,
                                                    "QML (*.qml)");
    if (fileName.size() > 0)
    {

        m_ui.m_declarativeView->rootContext()->engine()->clearComponentCache();

        //QMLVariable var;
        //m_ui.m_declarativeView->rootContext()->setContextProperty("sharedVariable",&var);



        createContextProperties();

        qDebug() << "Loading script :" << fileName;
        m_lastPath = fileName;

        //Do something with fileName...
        m_ui.m_declarativeView->setSource(QUrl::fromLocalFile(fileName));
        m_ui.m_declarativeView->show();

        qDebug("DeclarativeTest::loadButtonClicked() - Setting Property context : %p ",m_ui.m_declarativeView->rootContext());

    }

}

void DeclarativeTest::createContextProperties()
{
    qDebug("DeclarativeTest::createContextProperties()");

    Q_ASSERT(m_ui.m_declarativeView);

    QList<QObject*> allModules;

    for (int i = 0; i < m_view->getModules().size(); i++)
    {
        allModules.append(m_view->getModules()[i]);
    }

    m_ui.m_declarativeView->rootContext()->setContextProperty("moduleList",QVariant::fromValue(allModules));

    if (m_view->getModules().size() > 0)
    {
        ModuleVariable *var = m_view->getModules().first()->getVariable(0);
        if (var)
        {

            m_ui.m_declarativeView->rootContext()->setContextProperty("myVariable",var);

            qDebug("createContextProperties() - Setting Property context : %p ",m_ui.m_declarativeView->rootContext());
        }

    }
}

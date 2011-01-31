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

//This will insert the plugin in the dictionary...
static int declarative_test_plugin_init = BasePlugin::registerPlugin("DeclarativeTest",new BasePlugin::PluginFactory<DeclarativeTest>());



DeclarativeTest::DeclarativeTest(NetworkView *view)
	 : BasePlugin(view)
{
        qDebug("DeclarativeTest::DeclarativeTest(NetworkView *view = %p)",view);

	//Setup UI
	m_ui.setupUi(this);

        //Connect load button
        connect(m_ui.m_loadButton,SIGNAL(clicked()),this,SLOT(loadButtonClicked()));


        resize(800,600);
}

void DeclarativeTest::init()
{

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
            qDebug() << "Loading script :" << fileName;
            m_lastPath = fileName;

            //Do something with fileName...
            m_ui.m_declarativeView->setSource(QUrl::fromLocalFile(fileName));

            m_ui.m_declarativeView->show();

    }

}

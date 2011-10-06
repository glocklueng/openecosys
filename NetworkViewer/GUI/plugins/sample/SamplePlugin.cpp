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

#include "SamplePlugin.h"
#include "NetworkView.h"


//This will insert the plugin in the dictionary...
static int sample_plugin_init = BasePlugin::registerPlugin("SamplePlugin",new BasePlugin::PluginFactory<SamplePlugin>());



SamplePlugin::SamplePlugin(NetworkView *view)
	 : BasePlugin(view)
{
	qDebug("SamplePlugin::SamplePlugin(NetworkView *view = %p)",view);

	//Setup UI
	m_ui.setupUi(this);

	//Create timer
	m_timer = new QTimer(this);
	connect(m_timer,SIGNAL(timeout()),this,SLOT(timerUpdate()));
	m_timer->start(1000);
}

void SamplePlugin::init()
{

}

void SamplePlugin::terminate()
{

}

void SamplePlugin::timerUpdate()
{
	qDebug("void SamplePlugin::timerUpdate()");

	QList<NetworkModule*> allModules = m_view->getModules();

	m_ui.lcdNumber->display(allModules.size());
}

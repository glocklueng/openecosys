/**
     Copyright (C) 2009-2010 IntRoLab
     http://introlab.gel.usherbrooke.ca
     Dominic Letourneau, ing. M.Sc.A.
     Dominic.Letourneau@USherbrooke.ca
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

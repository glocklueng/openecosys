/**
     Copyright (C) 2009-2010 IntRoLab
     http://introlab.gel.usherbrooke.ca
     Dominic Letourneau, ing. M.Sc.A.
     Dominic.Letourneau@USherbrooke.ca
 */

#ifndef SAMPLEPLUGIN_H_
#define SAMPLEPLUGIN_H_

#include "BasePlugin.h"
#include "NetworkModule.h"
#include <QTimer>
#include "ui_SamplePlugin.h"


class SamplePlugin : public BasePlugin
{
	Q_OBJECT;


public:

	SamplePlugin(NetworkView *view);

	virtual void init();

	virtual void terminate();

protected slots:

	void timerUpdate();

protected:

	QTimer *m_timer;
	Ui::SamplePlugin m_ui;
	QList<NetworkModule*> m_modules;
};

#endif /* SAMPLEPLUGIN_H_ */

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

#ifndef _NETWORK_VIEWER_SPLASH_SCREEN_H_
#define _NETWORK_VIEWER_SPLASH_SCREEN_H_

#include <QSplashScreen>
#include "NetworkView.h"
#include <QTimer>

class NetworkViewerSplashScreen : public QSplashScreen
{

    Q_OBJECT;

    public:

    NetworkViewerSplashScreen()
    : QSplashScreen(QPixmap(":images/introlab.png"))
    {

        showMessage("http://www.openecosys.org",Qt::AlignRight);

	m_timer = new QTimer(this);
        connect(m_timer,SIGNAL(timeout()),this,SLOT(close()));

	//The splash screen will close after 2000 ms
	m_timer->start(2000);
    }

protected:

    QTimer *m_timer;

};

#endif


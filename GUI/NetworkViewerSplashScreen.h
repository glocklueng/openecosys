/**
     Copyright (C) 2009-2010 IntRoLab
     http://introlab.gel.usherbrooke.ca
     Dominic Letourneau, ing. M.Sc.A.
     Dominic.Letourneau@USherbrooke.ca
 */

#ifndef _NETWORK_VIEWER_SPLASH_SCREEN_H_
#define _NETWORK_VIEWER_SPLASH_SCREEN_H_

#include <QSplashScreen>
#include "NetworkView.h"

class NetworkViewerSplashScreen : public QSplashScreen
{

    Q_OBJECT;

    public:

    NetworkViewerSplashScreen()
    : QSplashScreen(QPixmap(":images/introlab.png"))
    {
        showMessage("Loading...");
    }


};

#endif


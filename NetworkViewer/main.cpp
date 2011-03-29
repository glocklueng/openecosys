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


#include <QApplication>
#include <QMessageBox>
#include <QtDebug>
#include <QEvent>
#include <QSplashScreen>
#include "ModuleConfiguration.h"
#include "ModuleConfigurationView.h"
#include "NetworkView.h"
#include "NetworkViewerSplashScreen.h"

void NetworkViewerMsgHandler(QtMsgType type, const char *msg);

class ConsoleMessageEvent : public QEvent    
{
public:

    ConsoleMessageEvent(const QString &message)
        : QEvent(QEvent::User)
    {
        m_message = message;
    }

public:

    QString m_message;
};




class NetworkViewerApp : public QApplication
{

public:

    NetworkViewerApp(int argc, char**argv)
        : QApplication(argc, argv), m_view(NULL), m_splashScreen(NULL)
    {

       

    }

    void init()
    {

        m_splashScreen = new NetworkViewerSplashScreen();
        m_splashScreen->show();

        qDebug() << "Running application from" << QCoreApplication::applicationDirPath ();


	    //Scan for all plugins
        //When we are in a test environment
        BasePlugin::scanPlugins(QCoreApplication::applicationDirPath() + "/plugins");
        BasePlugin::scanPlugins(QCoreApplication::applicationDirPath() + "/../plugins");


        //Scan for all drivers
        //When we are in a test environment
        NETVDevice::scanDrivers(QCoreApplication::applicationDirPath() + "/drivers");
        NETVDevice::scanDrivers(QCoreApplication::applicationDirPath() + "/../drivers");

       

        //Will wait for init...
        //m_debugWindow = new QMainWindow(NULL);
        //m_textEdit = new QTextEdit(m_debugWindow);
        //m_debugWindow->setCentralWidget(m_textEdit);
        //m_debugWindow->show();
        //m_debugWindow->raise();

        //Install handlers
        qInstallMsgHandler(NetworkViewerMsgHandler);


        m_view = new NetworkView(NULL);

        m_view->setWindowTitle(QString("NetworkViewer-") + NETWORKVIEWER_VERSION);


        if (m_splashScreen)
        {
            m_splashScreen->finish(m_view);
        }

        m_view->showMaximized();
    }

    ~NetworkViewerApp()
    {

        //Restore message handler
        qInstallMsgHandler(0);

        if (m_view)
        {
            delete m_view;
            m_view = NULL;
        }
    }

    bool event(QEvent* e)
    {
        if (e->type() == QEvent::User)
        {
            ConsoleMessageEvent *event = dynamic_cast<ConsoleMessageEvent*>(e);

            if (event && m_view)
            {
                m_view->printDebug(event->m_message);
                //m_textEdit->append(event->m_message);
            }

            return true;
        }

        return QApplication::event(e);
    }

protected:

    NetworkView *m_view;
    QMainWindow *m_debugWindow;
    QTextEdit *m_textEdit;
    NetworkViewerSplashScreen *m_splashScreen;

};



void NetworkViewerMsgHandler(QtMsgType type, const char *msg)
{
    NetworkViewerApp *app = dynamic_cast<NetworkViewerApp*>(QCoreApplication::instance());

    if (app)
    {
        switch (type) {
        case QtDebugMsg:
            QCoreApplication::postEvent(app,new ConsoleMessageEvent(QString("[DEBUG] ") + msg));
            break;
        case QtWarningMsg:
            QCoreApplication::postEvent(app,new ConsoleMessageEvent(QString("[WARNING] ") + msg));
            break;
        case QtCriticalMsg:
            QCoreApplication::postEvent(app,new ConsoleMessageEvent(QString("[CRITICAL] ") + msg));
            break;
        case QtFatalMsg:
            QCoreApplication::postEvent(app,new ConsoleMessageEvent(QString("[FATAL] ") + msg));
            abort();
        }
    }
}


int main(int argc, char* argv[])
{	
    NetworkViewerApp app(argc, argv);
    app.init();
    return app.exec();
}


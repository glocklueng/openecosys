
#include <QApplication>
#include <QDebug>
#include <QEvent>
#include "ModuleConfiguration.h"
#include "UserPreferences.h"
#include "NETVDevice.h"
#include <QList>
#include "NETVInterfaceManager.h"


class NETVConsoleApp : public QApplication
{
public:

    NETVConsoleApp(int argc, char** argv)
        : QApplication(argc,argv,false) //GUI Disabled
    {
        qDebug("Starting netvconsole...");

        //Scan for all drivers
        //When we are in a test environment
        NETVDevice::scanDrivers(QCoreApplication::applicationDirPath() + "/drivers");
        NETVDevice::scanDrivers(QCoreApplication::applicationDirPath() + "/../drivers");


        //Load user prefs
        loadUserPrefs();
    }

    ~NETVConsoleApp()
    {
        qDebug("Stopping nevconsole...");

        for (unsigned int i = 0; i <  m_interfaceManagerList.size(); i++)
        {
            delete m_interfaceManagerList[i];
        }

        m_interfaceManagerList.clear();

    }

 protected:

    void loadUserPrefs()
    {
        UserPreferences &prefs = UserPreferences::getGlobalPreferences();

        //Load prefs
        prefs.load();

        //Will load the interfaces configured by the NetworkViewer GUI
        if (prefs.contains("NETVInterfaceManagerView:nb_interface"))
        {
            int count = prefs.getKey("NETVInterfaceManagerView:nb_interface").toInt();

            //Let's create the interfaces...
            for (unsigned int i = 0; i < count; i++)
            {
                QString name_key = QString("NETVInterfaceManagerView:name_") + QString::number(i);
                QString args_key = QString("NETVInterfaceManagerView:args_") + QString::number(i);
                QString period_key = QString("NETVInterfaceManagerView:updatePeriod_") + QString::number(i);

                //Let's create the interface
                if (prefs.contains(name_key) && prefs.contains(args_key) && prefs.contains(period_key))
                {
                    QString name = prefs.getKey(name_key).toString();
                    QString args = prefs.getKey(args_key).toString();

                    int period = prefs.getKey(period_key).toInt();

                    NETVDevice *dev = NULL;


                    if(args.size())
                    {
                        dev = NETVDevice::createDevice(name, args.toStdString().c_str());
                    }
                    else
                    {
                        dev = NETVDevice::createDevice(name,NULL);
                    }

                    if (dev)
                    {
                        //Create manager
                        NETVInterfaceManager *manager = new NETVInterfaceManager(dev,NULL,this);

                        //Add manager
                        m_interfaceManagerList.push_back(manager);

                        //update scheduling
                        manager->getScheduler()->setVariableRequestInterval(period);
                    }
                }
            }
        }
        else
        {
            qDebug("No interface configured. Try configuring the interface with NetworkViewer GUI first. Exiting...");
            exit();
        }

    }


 private:

    ///Interface manager
    ///TODO Add multiple interface managers
    QList<NETVInterfaceManager*> m_interfaceManagerList;

};




int main(int argc, char* argv[])
{
    NETVConsoleApp app(argc,argv);
    app.exec();
    return 0;
}

/**
 */

#include "BasePlugin.h"
#include "NetworkView.h"
#include <QDir>


BasePlugin::BasePlugin(NetworkView *view)
    : QWidget(view), m_view(view)
{

}


QMap<QString, BasePlugin::BasePluginFactory*> & BasePlugin::loadedPlugins()
{
    static QMap<QString, BasePlugin::BasePluginFactory*> allPlugins;
    return allPlugins;
}


void BasePlugin::scanPlugins(const QString &basePath)
{


    qDebug() << "BasePlugin::scanPlugins() with basePath: "<<basePath;

    //Plugins directory
    QDir dir(basePath);

    if (!dir.exists())
    {
        qWarning("BasePlugin::scanPlugins : plugins directory not found");
        qWarning() << "BasePlugin::scanPlugins : Current path : " << dir.absolutePath();
    }
    else
    {
        qDebug() << "BasePlugin::scanPlugins : Scanning : " << dir.absolutePath();
        recursiveScan(dir);
    }


}

void BasePlugin::recursiveScan(QDir directory, int level)
{
    qDebug() << "BasePlugin::recursiveScan :  Path :" << directory.path();

    if (level < 10 && directory.exists())
    {

        QFileInfoList myInfoList = directory.entryInfoList();

        for (int i = 0; i < myInfoList.size(); i++)
        {

            if (!myInfoList[i].fileName().startsWith("."))
            {
                if (myInfoList[i].isDir())
                {
                    recursiveScan(QDir(directory.path() + QDir::separator() + myInfoList[i].fileName()), level + 1);
                }
                else
                {


                    //standard file
                    //TODO Unix dlls
#ifdef WIN32
                    if (myInfoList[i].fileName().contains(".dll") && !myInfoList[i].fileName().contains(".dll.a")) {
#else
                        if (myInfoList[i].fileName().contains(".so") || myInfoList[i].fileName().contains(".dylib")) {
#endif
                            qDebug() << "BasePlugin::recursiveScan : Loading library : " << directory.path() + QDir::separator() + myInfoList[i].fileName();
                            QLibrary *library = new QLibrary(directory.path() + QDir::separator() + myInfoList[i].fileName());

                            if (library->load())
                            {
                                qDebug() << "BasePlugin::recursiveScan : Loaded : " << directory.path() + QDir::separator() + myInfoList[i].fileName();
                                //TODO : here would be the place to call an init function for the library...
                                //TODO : do something with the library pointer...
                            }
                            else
                            {
                                qDebug() << "BasePlugin::recursiveScan : Error : " << library->errorString();
                            }
                        }
                    }
                }//starts with "."
            }//for infoList
	}
	else
	{
            qWarning("BasePlugin::recursiveScan : error level : %i",level);
	}


    }


    int BasePlugin::registerPlugin(const QString& name, BasePluginFactory *factory)
    {

	qDebug() << "BasePlugin::registerPlugin() : name : " << name << " factory: "<< factory;

	//QMap<QString, BasePluginFactory*> &allLibs = loadedPlugins();

	//Insert in the plugin map
	BasePlugin::loadedPlugins()[name] = factory;

	return BasePlugin::loadedPlugins().size();
    }

    void BasePlugin::closeRequest()
    {
       if(parentWidget())
       {
           parentWidget()->close();
       }
    }

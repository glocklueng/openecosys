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
#ifndef _BASEPLUGIN_H_
#define _BASEPLUGIN_H_

#include <QWidget>
#include <QMap>
#include <QLibrary>
#include <QDir>
#include <QEvent>
#include "BasePluginEvent.h"

//Forward Declaration of NetworkView
class NetworkView;

/**
 * Every plugin must inherit from this class. Basically, a plugin is a QWidget
 * that can be displayed in the application as a MDI component. Plugins are scanned
 * at startup, so they are created in separate dynamic libraries and they register
 * to the BasePlugin factory when loaded.
 *
 * \author Dominic Letourneau
 * \date Dec 4 2009
 */
class BasePlugin : public QWidget
{


public:

	/**
	 * Basic factory that can create a plugin. This is a virtual class that needs to be
	 * derived from.
	 */
	class BasePluginFactory
	{
		public:

		/**
		 * This virtual function must be implemented and is used to create
                 * A new plugin related to the \ref NetworkView.
		 * \param view the NetworkView.
		 */
		virtual BasePlugin* create(NetworkView *view) = 0;
	};

	/**
	 * Template class for quick and easy plugin factory creation.
	 * A factory can be created by using the template like this :
	 * BasePlugin::BasePluginFactory *myFactory = new BasePlugin::PluginFactory<class name>();
	 */
	template <class T>
	class PluginFactory : public BasePluginFactory
	{
	public:

		/**
		 * Overloaded from BasePluginFactory. This will create a new plugin
		 * \param view the NetworkView
		 */
		virtual BasePlugin* create(NetworkView *view)
		{
			return new T(view);
		}
	};

	/**
         *  BasePlugin constructor
	 *  \param view the NetworkView
	 */
        BasePlugin(NetworkView *view);

	/**
	 * 	NOT YET IMPLEMENTED, BUT PLUGINS NEED TO OVERLOAD THIS FUNCTION
	 *
	 */
	virtual void init() = 0;


	/**
	 *  NOT YET IMPLEMENTED, BUT PLUGINS NEED TO OVERLOAD THIS FUNCTION
	 *
	 */
	virtual void terminate() = 0;


        /**
            This will close the MDI window.
        */
        virtual void closeRequest();

	/**
	 *
	 * This will register a plugin with a name and a factory for plugin creation.
	 * This is a static function that can be called like this  :
	 * BasePlugin::registerPlugin("name",factory);
	 * \param name The plugin name
	 * \param factory The factory pointer
	 * \return int the number of factory / plugins present in the system
	 */
	static int registerPlugin(const QString &name, BasePluginFactory *factory);


	/**
	 * Will scan plugins (dll, so, dyld files) from a base directory. Will also
	 * traverse all sub-directory.
	 * \param basePath The path where to start the scan.
	 */
	static void scanPlugins(const QString &basePath);

	/**
	 * \return QMap<QString, BasePlugin::BasePluginFactory*>& The plugin dictionary.
	 *
	 */
 	static QMap<QString, BasePlugin::BasePluginFactory*> & loadedPlugins();



protected:

 	///The network view pointer
	NetworkView *m_view;

	/**
	 * Internal function to traverse directory for library scans.
	 * \param directory The current QDir
	 * \param level the recursion level, 0 = basePath
	 */
	static void recursiveScan(QDir directory, int level = 0);

};


#endif /* _BASEPLUGIN_H_ */

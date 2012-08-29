/**
     Copyright (C) 2009-2011 IntRoLab
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

#include "UserPreferences.h"




UserPreferences::UserPreferences()
{

}

bool UserPreferences::load()
{
    //Read prefs document
    QFile file(getPrefsPath());
    if (file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Prefs file opened : " << getPrefsPath();

        //Clear actual mal
        m_map.clear();

        QDomDocument doc("INTROLAB-NETWORKVIEWER-PREFS");

        //Set document contents
        if (!doc.setContent(&file)) {
            file.close();
            return false;
        }

        // <UserPreferences> element
        QDomElement e = doc.documentElement();
        if(e.tagName() != "UserPreferences")
        {
            qDebug() <<"No UserPreferences found. Element is " +  e.tagName();
            return false;
        }

        //Iterate through all child (this would be config_item)
        QDomNode n = e.firstChild();
        while(!n.isNull())
        {
            e = n.toElement(); // try to convert the node to an element.


            if(!e.isNull())
            {
                if(e.tagName() == "config_item")
                {
                    //Get Key
                    QString key = e.attribute("key");

                    qDebug() << "Loading : " << key;

                    //Get Value
                    QVariant value = e.attribute("value");

                    //Set Key/value
                    setKey(key,value,false);

                }
            }//if e.isNull
            else
            {
                qDebug("NULL element");
            }

            //Next item
            n = n.nextSibling();
        }//while n.isNull

        return true;
    }
    else
    {
        qDebug() << "Prefs file not found : " << getPrefsPath();
    }

    //File does not exist
    return false;
}

bool UserPreferences::save()
{
    QDomDocument document("INTROLAB-NETWORKVIEWER-PREFS");

    QDomElement element = document.createElement("UserPreferences");

    //Write every item
    for(QMap<QString, QVariant>::iterator iter = m_map.begin(); iter != m_map.end(); iter++)
    {
        QDomElement mapElement = document.createElement("config_item");
        mapElement.setAttribute("key",iter.key());
        mapElement.setAttribute("value",iter.value().toString());
        element.appendChild(mapElement);
    }

    document.appendChild(element);

    //Write document
    QFile file(getPrefsPath());
    if (file.open(QIODevice::WriteOnly))
    {
        QTextStream stream(&file);
        document.save(stream, 5);
        return true;
    }

    //Something went wrong
    return false;
}

bool UserPreferences::setKey(const QString &key, QVariant value, bool save_prefs)
{
    m_map[key] = value;

    //Saving changes
    if (save_prefs)
    {
        return save();
    }

    return true;
}

bool UserPreferences::contains(const QString &key)
{
    return m_map.contains(key);
}

QVariant UserPreferences::getKey(const QString &key)
{
    if (m_map.contains(key))
    {
        return m_map[key];
    }
    else
    {
        return QVariant::Invalid;
    }
}

int UserPreferences::size()
{
    return m_map.size();
}

UserPreferences& UserPreferences::getGlobalPreferences()
{
    static UserPreferences prefs;
    return prefs;
}

QString UserPreferences::getPrefsPath()
{
#ifdef __APPLE_CC__
    return QDir::homePath() + "/Documents/NetworkViewerPrefs.xml";
#else
    return QDir::homePath() + "/NetworkViewerPrefs.xml";
#endif
}



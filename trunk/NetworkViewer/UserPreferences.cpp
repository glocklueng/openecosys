/**
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
                    setKey(key,value);

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
    return QDir::homePath() + "/NetworkViewerPrefs.xml";
}



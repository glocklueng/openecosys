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

#ifndef _USER_PREFERENCES_H_
#define _USER_PREFERENCES_H_

#include <QObject>
#include <QVariant>

class UserPreferences : public QObject
{
    Q_OBJECT;
public:

    UserPreferences(QObject *parent)
        : QObject(parent)
    {

    }

    bool load(const QString &filename)
    {
        return false;
    }

    bool save(const QString &filename)
    {

    }

    bool setKey(const QString &key, QVariant value)
    {
        m_map[key] = value;
    }

    QVariant getKey(const QString &key)
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

protected:

    QMap<QString, QVariant> m_map;

};


#endif

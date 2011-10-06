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
#include <QDomDocument>
#include <QDir>
#include <QBuffer>
#include <QTextStream>
#include <QtDebug>

class UserPreferences
{

public:

    UserPreferences();

    bool load();

    bool save();

    bool setKey(const QString &key, QVariant value, bool save_prefs=true);

    bool contains(const QString &key);

    QVariant getKey(const QString &key);

    int size();

    static UserPreferences& getGlobalPreferences();

    static QString getPrefsPath();

protected:

    QMap<QString, QVariant> m_map;

};


#endif

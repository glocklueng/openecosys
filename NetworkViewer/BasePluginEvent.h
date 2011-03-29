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

#ifndef _BASE_PLUGIN_EVENT_H_
#define _BASE_PLUGIN_EVENT_H_

#include <QEvent>
#include <QMap>
#include <QString>
#include <QVariant>

#define NETV_PLUGIN_EVENT_TYPE (2000)


class BasePluginEvent : public QEvent
{
public:

    BasePluginEvent(const QString &key, const QVariant &value)
        : QEvent(BasePluginEvent::eventType())
    {
        m_map[key] = value;
    }

    QMap<QString,QVariant> m_map;

    //This will register the event type.
    static QEvent::Type eventType();
};


#endif

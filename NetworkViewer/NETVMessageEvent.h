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

#ifndef _CAN_MESSAGE_EVENT_H_
#define _CAN_MESSAGE_EVENT_H_

#include <QEvent>

/**
    An user event that is used for signaling a new message from a thread to another thread with the Qt event system.
*/
class NETVMessageEvent : public QEvent
{
    public:

    /**
        Constructor
        \param msg the message to put in the event
     */
    NETVMessageEvent(const NETV_MESSAGE &msg)
    : QEvent(QEvent::User), m_message(msg)
    {

    }

    /**
        \return NETV_MESSAGE the contained message
    */
    NETV_MESSAGE getMessage() {return m_message;}
   
    protected:

    ///The message
    NETV_MESSAGE m_message;
};


#endif


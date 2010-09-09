/**
     Copyright (C) 2009-2010 IntRoLab
     http://introlab.gel.usherbrooke.ca
     Dominic Letourneau, ing. M.Sc.A.
     Dominic.Letourneau@USherbrooke.ca
 */

#ifndef _CAN_MESSAGE_EVENT_H_
#define _CAN_MESSAGE_EVENT_H_

#include <QEvent>

/**
    An user event that is used for signaling a new message from a thread to another thread with the Qt event system.
*/
class CANMessageEvent : public QEvent
{
    public:

    /**
        Constructor
        \param msg the message to put in the event
     */
    CANMessageEvent(const LABORIUS_MESSAGE &msg)
    : QEvent(QEvent::User), m_message(msg)
    {

    }

    /**
        \return LABORIUS_MESSAGE the contained message
    */
    LABORIUS_MESSAGE getMessage() {return m_message;}
   
    protected:

    ///The message
    LABORIUS_MESSAGE m_message;
};


#endif


/**
     Copyright 2011 (C) Dominic Letourneau, ing. M.Sc.A.
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

#ifndef _SOCKET_CAN_H_
#define _SOCKET_CAN_H_


#include "NETVDevice.h"
#include <QSemaphore>
#include <QMutex>
#include <QList>
#include <QEvent>
#include <QTimer>
#include <QTime>
#include "NETV_define.h"


class SocketCAN : public QObject, public NETVDevice
{
	Q_OBJECT;

	public:

       


	SocketCAN(const char* params);

	virtual ~SocketCAN();
	
	virtual NETVDevice::State initialize(const char* args);


	  /** send a NETV_MESSAGE
		   \param message The message to send
		   \return int The status after the message has been sent
	  */
	  virtual NETVDevice::State sendMessage(NETV_MESSAGE &message);

	  /** receive a NETV_MESSAGE
		   \param message The message to receive (will be filled)
		   \return int The status after the message has been received
	  */
	  virtual NETVDevice::State recvMessage(NETV_MESSAGE &message);
	
	/** Verify if a message is ready to receive
	 \return bool true if a message is ready to be received
	 */
	virtual bool newMessageReady();

	protected slots:
	
	protected:
	



};


#endif


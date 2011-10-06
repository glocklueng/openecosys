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

#ifndef _LOOPBACK_H_
#define _LOOPBACK_H_


#include "NETVDevice.h"
#include <QSemaphore>
#include <QMutex>
#include <QList>
#include <QEvent>
#include <QTimer>
#include <QTime>
#include "NETV_define.h"


class Loopback : public QObject, public NETVDevice
{
	Q_OBJECT;

	public:

        class VirtualModule
        {

        public:


            static const int NB_VARIABLES=4;

            VirtualModule(int id)
                : module_id(id)
            {
                project_id = 0;
                code_version = 1;
                table_version = 2;
                device_id = 0;
                state = NETV_NORMAL_MODE_ID;
                for (unsigned int i = 0 ; i < NB_VARIABLES; i++)
                {
                    variable[i] = 0;
                }
            }

            int module_id;
            int project_id;
            int code_version;
            int table_version;
            int device_id;
            int state;

            union {

                struct {
                    double variable[NB_VARIABLES];
                };

                unsigned char data[NB_VARIABLES *sizeof(double)];

            };

        };


	Loopback(const char* params);

	virtual ~Loopback();
	
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
	
        void timeout();

	protected:
	
        QSemaphore m_semaphore;
        QMutex m_mutex;
        QList<NETV_MESSAGE> m_messageList;
        QList<VirtualModule> m_moduleList;
        QTimer *m_updateTimer;
        QTime m_time;

};


#endif


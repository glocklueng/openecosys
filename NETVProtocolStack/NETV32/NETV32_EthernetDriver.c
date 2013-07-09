/*
The OpenEcoSys project / NETVProtocolStack
Copyright (C) 2013  IntRoLab - Universite de Sherbrooke

Author(s)

Dominic Letourneau, ing., M.Sc.A.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "NETV32_Common.h"
#include "NETV32_Utils.h"
#include "TCPIPConfig.h"
#include "TCPIP Stack/TCPIP.h"

static TCP_SOCKET MySocket = INVALID_SOCKET;



// Defines which port the server will listen on
unsigned int g_SERVER_PORT = 9760;


int netv_init_ethernet(unsigned int port)
{
	g_SERVER_PORT = port;
	return 0;
}

void NETV32_SocketServer(void)


{
	static enum _TCPServerState
	{
		SM_HOME = 0,
		SM_LISTENING,
        SM_CLOSING,
	} TCPServerState = SM_HOME;

	switch(TCPServerState)
	{
		case SM_HOME:
			// Allocate a socket for this server to listen and accept connections on
			MySocket = TCPOpen(0, TCP_OPEN_SERVER, g_SERVER_PORT, TCP_PURPOSE_DEFAULT);
			if(MySocket == INVALID_SOCKET)
				return;

			TCPServerState = SM_LISTENING;
			break;

		case SM_LISTENING:
			// See if anyone is connected to us
			if(!TCPIsConnected(MySocket))
				return;

			break;

		case SM_CLOSING:
			// Close the socket connection.
            TCPClose(MySocket);

			TCPServerState = SM_HOME;
			break;
	}
}


unsigned char netv_send_message(NETV_MESSAGE *message)
{

    int i =0;
    CANTxMessageBuffer CANBuffer;

    //Test for connect
    if(!TCPIsConnected(MySocket))
		return 1;


    if (TCPIsPutReady(MySocket) >= sizeof(CANTxMessageBuffer))
    {

        memset((char*) &CANBuffer,0,sizeof(CANTxMessageBuffer));


        unsigned int ID = 0;

        //priority
        ID |= (((unsigned int)message->msg_priority << 26) & 0x1C000000);

        //type
        ID |= (((unsigned int)message->msg_type << 18) & 0x03FC0000);

        //boot        
        ID |= (((unsigned int)message->msg_eeprom_ram << 17) & 0x00020000);
        ID |= (((unsigned int)message->msg_read_write << 16) & 0x00010000);

        //cmd
        ID |= (((unsigned int)message->msg_cmd << 8) & 0x0000FF00);

        //dest
        ID |= (((unsigned int)message->msg_dest) & 0x000000FF);


        CANBuffer.msgSID.SID = (ID >> 18);
        CANBuffer.msgEID.EID = (ID & 0x0003FFFF);

        //Set the interface (default to 1)
        //CANBuffer.msgSID.IFACE = 0;

        //Set extended message
        CANBuffer.msgEID.IDE = 1;

        //Those bits should always be cleared
        CANBuffer.msgEID.SRR = 0;
        CANBuffer.msgEID.RB0 = 0;
        CANBuffer.msgEID.RB1 = 0;

        //Set RTR
        CANBuffer.msgEID.RTR = message->msg_remote;

        //copy data length
        CANBuffer.msgEID.DLC = message->msg_data_length;

        //copy data
        for (i = 0; i<8; i++)
        {
                CANBuffer.data[i] = message->msg_data[i];
        }

        //Write to socket
        TCPPutArray(MySocket, (char*) &CANBuffer, sizeof(CANTxMessageBuffer));

        //No need to flush the socket

        return 0;
    }


    //fail
    return 1;
}



unsigned char netv_recv_message(NETV_MESSAGE *message)
{

    CANRxMessageBuffer buf;
    int i = 0;

    //Test for connect
    if(!TCPIsConnected(MySocket))
		return 0;

    if (TCPIsGetReady(MySocket) >= sizeof(CANRxMessageBuffer))
    {

        TCPGetArray(MySocket, (char*) &buf, sizeof(CANRxMessageBuffer));


        //Copy message...
        unsigned int SID = (buf.messageWord[0]) & 0x000007FF;
        unsigned int EID = (buf.messageWord[1] >> 10) & 0x0003FFFF;

        //Here convert the message buffer to a NETV_CAN message
        message->msg_priority = (SID >> 8) & 0x07;
        message->msg_type = (SID) & 0xFF;
        message->msg_eeprom_ram = (EID >> 17) & 0x01;
        message->msg_read_write = (EID >> 16) & 0x01;
        message->msg_cmd = (EID >> 8) & 0xFF;
        message->msg_dest = (EID) & 0xFF;
        message->msg_data_length = buf.msgEID.DLC;

        //copy data
        for (i = 0; i < MIN(8, buf.msgEID.DLC); i++) {
            message->msg_data[i] = buf.data[i];
        }

        message->msg_remote = buf.msgEID.RTR;

        //ok
        return 1;
    }

    //fail
    return 0;
}
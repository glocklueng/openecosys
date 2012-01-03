#include "NETVBroadcaster.h"
#include <QtDebug>
#include <QBuffer>



NETVBroadcaster::NETVBroadcaster(int udp_port, int tcp_port, QObject *parent)
    :   QObject(parent), m_udpPort(udp_port), m_tcpPort(tcp_port)
{

    qDebug("NETVBroadcaster starting on port: %i, tcp: %i", m_udpPort, m_tcpPort);

    //Create socket
    m_socket = new QUdpSocket(this);
    m_socket->bind(m_udpPort,QUdpSocket::DefaultForPlatform);
    m_timerID = startTimer(1000);//1000 ms
}

void NETVBroadcaster::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_timerID)
    {
        //We should broadcast...
        QBuffer buffer;
        buffer.open(QIODevice::WriteOnly);
        m_info.serialize(buffer);

        //Broadcast information
        qDebug("Broadcasting size: %i",buffer.data().size());
        qDebug() << buffer.data();
        m_socket->writeDatagram(buffer.data(),QHostAddress::Broadcast, m_udpPort);

    }
}



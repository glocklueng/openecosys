#ifndef _NETVBROADCASTER_H_
#define _NETVBROADCASTER_H_

#include <QUdpSocket>
#include <QTimer>
#include <QTimerEvent>
#include <QIODevice>
#include <QDomDocument>
#include <QDomElement>
#include <QTextStream>

class NETVRemoteInfo : public QObject
{
    Q_OBJECT;

public:

    NETVRemoteInfo(QObject *parent=NULL)
        : QObject(parent), m_name("unnamed"), m_description("no description"), m_port(-1)
    {

    }


    bool serialize(QIODevice &dev)
    {

        if (!dev.isWritable())
        {
            qDebug("NETVRemoteInfo::serialize - Device is not writeable");
            return false;
        }

        QDomDocument doc("NETVRemoteInfo");
        QDomElement generalElement = doc.createElement("general-info");

        //Name
        generalElement.setAttribute("name",m_name);
        //Description
        generalElement.setAttribute("description",m_description);
        //Port
        generalElement.setAttribute("port",QString::number(m_port));

        doc.appendChild(generalElement);


        QTextStream stream(&dev);
        doc.save(stream, 4);
        return true;
    }

    bool unserialize(QIODevice &dev)
    {
        return false;
    }


    QString m_name;
    QString m_description;
    int m_port;
};



class NETVBroadcaster : public QObject
{
    Q_OBJECT;

public:

    NETVBroadcaster(int udp_port=8888, int tcp_port=12345, QObject *parent=NULL);


protected slots:

   virtual void timerEvent(QTimerEvent *event);


protected:
    int m_timerID;
    int m_tcpPort;
    int m_udpPort;
    QUdpSocket *m_socket;
    NETVRemoteInfo m_info;

};


#endif

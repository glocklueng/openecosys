
#include "qextserialhandshake.h"

#include "qextserialenumerator.h"
#include "qextserialport.h"
#include <QRegExp>
#include <QtCore/QTime>
#include <QtCore/QtConcurrentFilter>

struct Shaker
{
    typedef bool result_type;

    /*!
    */
    bool operator()(QextSerialHandShake::pair_type &pair) const
    {
        QextSerialPort port(pair.first.portName, QextSerialPort::Polling);

        QList<QextPortSettings>::const_iterator itr;
        QList<QextPortSettings>::const_iterator end = settings.end();
        for (itr = settings.begin(); proceed() && (itr != end); ++itr) {
            port.setPortSettings(*itr);
            if (port.open(QextSerialPort::ReadWrite)) {
                bool success = true;
                QList<QByteArray>::size_type size = qMax(commands.size(), responses.size());
                for (QList<QByteArray>::size_type i = 0; proceed() && (i < size); ++i) {
                    port.write(commands.at(i));
                    if (!readUntil(port, responses.at(i))) {
                        success = false;
                        break;
                    }
                }

                if (success) {
                    found.ref();
                    pair.second = *itr;
                }

                port.close();
                if (success)
                    return true;
            }
        }

        return false;
    }

    /*!
    */
    bool proceed() const
    {
        return !(stopOnFirst && (found > 0));
    }

    /*!
    */
    bool readUntil (QextSerialPort &port, QRegExp regex) const
    {
        if (!regex.isValid()) {
            qWarning("Shaker: regular expression is invalid: %s", qPrintable(regex.errorString()));
            return false;
        }

        QTime time;
        QByteArray buffer;

        time.start();
        while (proceed() && (static_cast<quint32>(time.elapsed()) < timeout)) {
            if (port.bytesAvailable() > 0) {
                buffer.append(port.readAll());
                if (regex.indexIn(QString::fromAscii(buffer.constData(), buffer.size())) != -1)
                    return true;
            }
        }
        return false;
    }

    bool stopOnFirst;
    quint32 timeout;
    QList<QRegExp> responses;
    QList<QByteArray> commands;
    QList<QextPortSettings> settings;
    mutable QAtomicInt found;
};

/*!
*/
QextSerialHandShake::QextSerialHandShake()
{
}

/*!
*/
void QextSerialHandShake::setTestPorts(const QList<QextPortInfo> &ports)
{
    m_ports = ports;
}

/*!
*/
void QextSerialHandShake::setTestSettings(const QList<QextPortSettings> &settings)
{
    m_settings = settings;
}

/*!
*/
QextSerialHandShake::result_type QextSerialHandShake::findPorts (const QByteArray &command,
    const QRegExp &response, quint32 timeout, bool stopOnFirst) const
{
    QList<QByteArray> commands = QList<QByteArray>() << command;
    QList<QRegExp> responses = QList<QRegExp>() << response;
    return findPorts(commands, responses, timeout, stopOnFirst);
}

/*!
*/
QextSerialHandShake::result_type QextSerialHandShake::findPorts (const QList<QByteArray> &commands,
    const QList<QRegExp> &responses, quint32 timeout, bool stopOnFirst) const
{
    if (commands.isEmpty() || responses.isEmpty())
        return QextSerialHandShake::result_type();

    QList<QextPortInfo> results;
    QList<QextPortInfo> ports(m_ports);
    QList<QextPortSettings> settings(m_settings);

    if (ports.isEmpty())
        ports = QextSerialEnumerator::getPorts();
    if (settings.isEmpty())
        settings.push_back(QextPortSettings());

    Shaker shaker;
    shaker.timeout = timeout;
    shaker.stopOnFirst = stopOnFirst;
    shaker.commands = commands;
    shaker.responses = responses;
    shaker.settings = settings;

    result_type sequence;
    QList<QextPortInfo>::iterator end = ports.end();
    for (QList<QextPortInfo>::iterator itr = ports.begin(); itr != end; ++itr)
        sequence.push_back(pair_type(*itr, QextPortSettings()));

    QFuture<pair_type> future = QtConcurrent::filtered(sequence.begin(), sequence.end(), shaker);
    future.waitForFinished();

    return future.results();
}

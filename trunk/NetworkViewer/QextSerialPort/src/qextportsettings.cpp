
#include "qextportsettings.h"

#include <QtCore/QDataStream>
#include <QtCore/QRegExp>

/*!
    Default constructor. This initializes the QextPortSettings object to default values.
*/
QextPortSettings::QextPortSettings()
    : baudRate(BAUD115200),
    dataBits(DATA_8),
    parity(PAR_NONE),
    stopBits(STOP_1),
    flowControl(FLOW_HARDWARE),
    timeoutMsecs(500)
{
}

/*!
    Converts the QextPortSettings object to a QString. The string can later be parsed using
    QextPortSettings::fromString.
*/
QString QextPortSettings::toString () const
{
    QString str;

        // determine and write baud rate
    qint32 value = 115200;
    switch (baudRate) {
    case BAUD50:
        value = 50;
        break;
    case BAUD75:
        value = 75;
        break;
    case BAUD110:
        value = 110;
        break;
    case BAUD134:
        value = 134;
        break;
    case BAUD150:
        value = 150;
        break;
    case BAUD200:
        value = 200;
        break;
    case BAUD300:
        value = 300;
        break;
    case BAUD600:
        value = 600;
        break;
    case BAUD1200:
        value = 1200;
        break;
    case BAUD1800:
        value = 1800;
        break;
    case BAUD2400:
        value = 2400;
        break;
    case BAUD4800:
        value = 4800;
        break;
    case BAUD9600:
        value = 9600;
        break;
    case BAUD14400:
        value = 14400;
        break;
    case BAUD19200:
        value = 19200;
        break;
    case BAUD38400:
        value = 38400;
        break;
    case BAUD56000:
        value = 56000;
        break;
    case BAUD57600:
        value = 57600;
        break;
    case BAUD76800:
        value = 76800;
        break;
    case BAUD115200:
        value = 115200;
        break;
    case BAUD128000:
        value = 128000;
        break;
    case BAUD256000:
        value = 256000;
        break;
    default:
        qWarning("QextPortSettings: unrecognized baud rate enumeration, %d.", baudRate);
        break;
    }
    str.append(QString("baudrate=%1 ").arg(value));

        // determine and write data bits
    value = 8;
    switch (dataBits) {
    case DATA_5:
        value = 5;
        break;
    case DATA_6:
        value = 6;
        break;
    case DATA_7:
        value = 7;
        break;
    case DATA_8:
        value = 8;
        break;
    default:
        qWarning("QextPortSettings: unrecognized data bits enumeration, %d.", dataBits);
        break;
    }
    str.append(QString("databits=%1 ").arg(value));

        // determine and write parity
    switch (parity) {
    case PAR_NONE:
        str.append("parity=none ");
        break;
    case PAR_ODD:
        str.append("parity=odd ");
        break;
    case PAR_EVEN:
        str.append("parity=even ");
        break;
    case PAR_MARK:
        str.append("parity=mark ");
        break;
    case PAR_SPACE:
        str.append("parity=space ");
        break;
    default:
        str.append("parity=none ");
        qWarning("QextPortSettings: unrecognized parity enumeration, %d.", dataBits);
        break;
    }

        // determine and write stop bits
    switch (stopBits) {
    case STOP_1:
        str.append("stopbits=1 ");
        break;
    case STOP_1_5:
        str.append("stopbits=1.5 ");
        break;
    case STOP_2:
        str.append("stopbits=2 ");
    default:
        str.append("stopbits=1 ");
        qWarning("QextPortSettings: unrecognized stop bits enumeration, %d.", dataBits);
        break;
    }

    // determine and write flow control
    switch (flowControl) {
    case FLOW_OFF:
        str.append("flowctrl=off ");
        break;
    case FLOW_HARDWARE:
        str.append("flowctrl=hardware ");
        break;
    case FLOW_XONXOFF:
        str.append("flowctrl=software ");
    default:
        str.append("flowctrl=hardware ");
        qWarning("QextPortSettings: unrecognized flow control enumeration, %d.", dataBits);
        break;
    }
 

    str.append(QString("timeout=%1").arg(timeoutMsecs));

    return str;
}

/*!
    Parses \a str and creates a QextPortSettings object. The string should be in the format returned
    by QextPortSettings::toString.
*/
QextPortSettings QextPortSettings::fromString (const QString &str)
{
    QRegExp regex("(\\w+)(?:=)(\\w+)");
    QextPortSettings settings;

        // loop through each key and value pair
    int pos = regex.indexIn(str);
    while (pos >= 0) {
            // get the key and value strings
        QString key = regex.cap(1);
        QString value = regex.cap(2);

            // find the key and then parse for the corresponding value
        if (key.compare("baudrate", Qt::CaseInsensitive) == 0) {
            bool parsed = true;
            unsigned int baudRate = value.toInt(&parsed);
            if (parsed) {
                switch (baudRate) {
                case 50:
                    settings.baudRate = BAUD50;
                    break;
                case 75:
                    settings.baudRate = BAUD75;
                    break;
                case 110:
                    settings.baudRate = BAUD110;
                    break;
                case 134:
                    settings.baudRate = BAUD134;
                    break;
                case 150:
                    settings.baudRate = BAUD150;
                    break;
                case 200:
                    settings.baudRate = BAUD200;
                    break;
                case 300:
                    settings.baudRate = BAUD300;
                    break;
                case 600:
                    settings.baudRate = BAUD600;
                    break;
                case 1200:
                    settings.baudRate = BAUD1200;
                    break;
                case 1800:
                    settings.baudRate = BAUD1800;
                    break;
                case 2400:
                    settings.baudRate = BAUD2400;
                    break;
                case 4800:
                    settings.baudRate = BAUD4800;
                    break;
                case 9600:
                    settings.baudRate = BAUD9600;
                    break;
                case 14400:
                    settings.baudRate = BAUD14400;
                    break;
                case 19200:
                    settings.baudRate = BAUD19200;
                    break;
                case 38400:
                    settings.baudRate = BAUD38400;
                    break;
                case 56000:
                    settings.baudRate = BAUD56000;
                    break;
                case 57600:
                    settings.baudRate = BAUD57600;
                    break;
                case 76800:
                    settings.baudRate = BAUD76800;
                    break;
                case 115200:
                    settings.baudRate = BAUD115200;
                    break;
                case 128000:
                    settings.baudRate = BAUD128000;
                    break;
                case 256000:
                    settings.baudRate = BAUD256000;
                    break;
                }
            }
        } else if (key.compare("databits", Qt::CaseInsensitive) == 0) {
            bool parsed = true;
            unsigned int dataBits = value.toInt(&parsed);
            if (parsed) {
                switch (dataBits) {
                case 5:
                    settings.dataBits = DATA_5;
                    break;
                case 6:
                    settings.dataBits = DATA_6;
                    break;
                case 7:
                    settings.dataBits = DATA_7;
                    break;
                case 8:
                    settings.dataBits = DATA_8;
                    break;
                }
            }
        } else if (key.compare("parity", Qt::CaseInsensitive) == 0) {
            if (value.compare("none", Qt::CaseInsensitive) == 0) {
                settings.parity = PAR_NONE;
            } else if (value.compare("odd", Qt::CaseInsensitive) == 0) {
                settings.parity = PAR_ODD;
            } else if (value.compare("even", Qt::CaseInsensitive) == 0) {
                settings.parity = PAR_EVEN;
            } else if (value.compare("mark", Qt::CaseInsensitive) == 0) {
                settings.parity = PAR_MARK;
            } else if (value.compare("space", Qt::CaseInsensitive) == 0) {
                settings.parity = PAR_SPACE;
            }
        } else if (key.compare("stopbits", Qt::CaseInsensitive) == 0) {
            if (value.compare("1", Qt::CaseInsensitive) == 0) {
                settings.stopBits = STOP_1;
            } else if (value.compare("1.5", Qt::CaseInsensitive) == 0) {
                settings.stopBits = STOP_1_5;
            } else if (value.compare("2", Qt::CaseInsensitive) == 0) {
                settings.stopBits = STOP_2;
            }
        } else if (key.compare("flowctrl", Qt::CaseInsensitive) == 0) {
            if (value.compare("off", Qt::CaseInsensitive) == 0) {
                settings.flowControl = FLOW_OFF;
            } else if (value.compare("hardware", Qt::CaseInsensitive) == 0) {
                settings.flowControl = FLOW_HARDWARE;
            } else if (value.compare("software", Qt::CaseInsensitive) == 0) {
                settings.flowControl = FLOW_XONXOFF;
            }
        } else if (key.compare("timeout", Qt::CaseInsensitive) == 0) {
            bool parsed = true;
            qint64 timeout = value.toLongLong(&parsed);
            if (parsed)
                settings.timeoutMsecs = timeout;
        }

            // find the next key and value pair
        pos += regex.matchedLength();
        pos = regex.indexIn(str, pos);
    }

    return settings;
}

#ifndef QT_NO_DATASTREAM
/*!
    Writes the \a settings to the \a stream, and returns a reference to the stream.
*/
QDataStream &operator<<(QDataStream &stream, const QextPortSettings &settings)
{
    stream << qint8(settings.baudRate) << qint8(settings.dataBits) << qint8(settings.parity)
        << qint8(settings.stopBits) << qint8(settings.flowControl) << settings.timeoutMsecs;
    return stream;
}

/*!
    Reads a \a settings from the \a stream, and returns a reference to the stream.
*/
QDataStream &operator>>(QDataStream &stream, QextPortSettings &settings)
{
    qint8 baudRate;
    qint8 dataBits;
    qint8 parity;
    qint8 stopBits;
    qint8 flowControl;
    qint64 timeout;
    stream >> baudRate >> dataBits >> parity >> stopBits >> flowControl >> timeout;

    settings.baudRate = static_cast<BaudRateType>(baudRate);
    settings.dataBits = static_cast<DataBitsType>(dataBits);
    settings.parity = static_cast<ParityType>(parity);
    settings.stopBits = static_cast<StopBitsType>(stopBits);
    settings.flowControl = static_cast<FlowType>(flowControl);
    settings.timeoutMsecs = timeout;

    return stream;
}

#endif // QT_NO_DATASTREAM

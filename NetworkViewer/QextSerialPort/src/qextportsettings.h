
#ifndef _QEXTPORTSETTINGS_H_
#define _QEXTPORTSETTINGS_H_

#include "qextserialport_global.h"

enum BaudRateType
{
    BAUD50=50,                //POSIX ONLY
    BAUD75=75,                //POSIX ONLY
    BAUD110=110,
    BAUD134=134,               //POSIX ONLY
    BAUD150=150,               //POSIX ONLY
    BAUD200=200,               //POSIX ONLY
    BAUD300=300,
    BAUD600=600,
    BAUD1200=1200,
    BAUD1800=1800,              //POSIX ONLY
    BAUD2400=2400,
    BAUD4800=4800,
    BAUD9600=9600,
    BAUD14400=14400,             //WINDOWS ONLY
    BAUD19200=19200,
    BAUD38400=38400,
    BAUD56000=56000,             //WINDOWS ONLY
    BAUD57600=57600,
    BAUD76800=76800,             //POSIX ONLY
    BAUD115200=115200,
    BAUD128000=128000,            //WINDOWS ONLY
    BAUD256000=256000             //WINDOWS ONLY
};

enum DataBitsType
{
    DATA_5,
    DATA_6,
    DATA_7,
    DATA_8
};

enum ParityType
{
    PAR_NONE,
    PAR_ODD,
    PAR_EVEN,
    PAR_MARK,               //WINDOWS ONLY
    PAR_SPACE
};

enum StopBitsType
{
    STOP_1,
    STOP_1_5,               //WINDOWS ONLY
    STOP_2
};

enum FlowType
{
    FLOW_OFF,
    FLOW_HARDWARE,
    FLOW_XONXOFF
};

/*!
Stores serial port configuration settings. This class contains functions to store the settings as a
QString, or write to a QDataStream. Both of these operations have corresponding read functions. 
*/
struct QEXTSERIALPORT_EXPORT QextPortSettings
{
    QextPortSettings();

    QString toString () const;

    static QextPortSettings fromString (const QString &str);

    BaudRateType baudRate;
    DataBitsType dataBits;
    ParityType parity;
    StopBitsType stopBits;
    FlowType flowControl;
    qint64 timeoutMsecs;
};

#ifndef QT_NO_DATASTREAM
QEXTSERIALPORT_EXPORT QDataStream &operator<<(QDataStream &stream, const QextPortSettings &settings);
QEXTSERIALPORT_EXPORT QDataStream &operator>>(QDataStream &stream, QextPortSettings &settings);
#endif

#endif

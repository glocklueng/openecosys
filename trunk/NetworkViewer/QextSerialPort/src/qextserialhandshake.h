
#ifndef _QEXTSERIALHANDSHAKE_H_
#define _QEXTSERIALHANDSHAKE_H_

#include "qextserialport_global.h"

#include "qextportinfo.h"
#include "qextportsettings.h"

#include <QtCore/QList>
#include <QtCore/QPair>

QT_FORWARD_DECLARE_CLASS(QRegExp)

/*!
*/
class QEXTSERIALPORT_EXPORT QextSerialHandShake
{
public:
    typedef QPair<QextPortInfo, QextPortSettings> pair_type;
    typedef QList<pair_type>  result_type;

    QextSerialHandShake();

    void setTestPorts (const QList<QextPortInfo> &ports);
    void setTestSettings (const QList<QextPortSettings> &settings);

    result_type findPorts (const QByteArray &message, const QRegExp &response,
        quint32 timeout = 500, bool stopOnFirst = false) const;

    result_type findPorts (const QList<QByteArray> &messages, const QList<QRegExp> &responses,
        quint32 timeout = 500, bool stopOnFirst = false) const;

private:
    QList<QextPortInfo> m_ports;
    QList<QextPortSettings> m_settings;
};

#endif

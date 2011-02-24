
#ifndef _QEXTPORTINFO_H_
#define _QEXTPORTINFO_H_

#include "qextserialport_global.h"

#include <QtCore/QString>

/*!
 * Structure containing port information.
 */
struct QEXTSERIALPORT_EXPORT QextPortInfo {
    QextPortInfo();

    QString portName;   ///< Port name.
    QString physName;   ///< Physical name.
    QString friendName; ///< Friendly name.
    QString enumName;   ///< Enumerator name.
    int vendorID;       ///< Vendor ID.
    int productID;      ///< Product ID
};

#endif

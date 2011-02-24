#ifndef _SERIAL_EMULATOR_CONFIGURE_H_
#define _SERIAL_EMULATOR_CONFIGURE_H_

#include <QDialog>


#include "ui_SerialEmulatorConfigure.h"

class SerialEmulatorConfigure : public QDialog, public Ui::SerialEmulatorConfigure
{
    Q_OBJECT;

public:

    SerialEmulatorConfigure(QWidget *parent = NULL);

};



#endif

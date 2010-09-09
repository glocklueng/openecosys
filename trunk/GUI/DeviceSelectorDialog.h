/**
     Copyright (C) 2009-2010 IntRoLab
     http://introlab.gel.usherbrooke.ca
     Dominic Letourneau, ing. M.Sc.A.
     Dominic.Letourneau@USherbrooke.ca
 */

#ifndef _DEVICE_SELECTOR_DIALOG_H_
#define _DEVICE_SELECTOR_DIALOG_H_

#include "ui_DeviceSelectorDialog.h"
#include <QDialog>
#include <QString>
#include "CANDevice.h"

///Forward declaration of NetworkView
class NetworkView;


/**
    Simple Dialog used for device selection. This may change in the future according to loaded device drivers.
    \todo The dialog should be dynamically created.
    \author Dominic Letourneau
    \date May 2010
*/
class DeviceSelectorDialog : public QDialog, public Ui::DeviceSelectorDialog
{

    Q_OBJECT;

    public:

    /**
        Constructor
        \param view The \ref NetworkView (Main Window)
    */
    DeviceSelectorDialog(NetworkView *view);


    /**
        \return QString the currently selected device
    */
    QString selectedDevice();

    /**
        \return The driver current args
    */    
    QString args();

    /**

        \return The driver documentation
    */
    QString documentation();


    /**
        \return CANDevice::BaseDeviceFactory* The current factory associated with the selection
    */
    CANDevice::BaseDeviceFactory* getFactory();


    protected slots:

    ///Device selector slot, called when we change the device
    void comboActivated(int index);

    protected:

    ///The NetworkView (Main Window)
    NetworkView *m_view;

    ///The current factory
    CANDevice::BaseDeviceFactory* m_factory;
	
};


#endif




/**
     Copyright (C) 2009-2010 IntRoLab
     http://introlab.gel.usherbrooke.ca
     Dominic Letourneau, ing. M.Sc.A.
     Dominic.Letourneau@USherbrooke.ca

     This file is part of OpenECoSys/NetworkViewer.
     OpenECoSys/NetworkViewer is free software: you can redistribute it and/or modify
     it under the terms of the GNU General Public License as published by the Free Software
     Foundation, either version 3 of the License, or (at your option) any later version.
     OpenECoSys/NetworkViewer is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
     or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
     You should have received a copy of the GNU General Public License along with
     OpenECoSys/NetworkViewer. If not, see http://www.gnu.org/licenses/.

 */

#ifndef _DEVICE_SELECTOR_DIALOG_H_
#define _DEVICE_SELECTOR_DIALOG_H_

#include "ui_DeviceSelectorDialog.h"
#include <QDialog>
#include <QString>
#include "NETVDevice.h"

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
        \param parent The parent widget
    */
    DeviceSelectorDialog(QWidget *parent=NULL);


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
        \return NETVDevice::BaseDeviceFactory* The current factory associated with the selection
    */
    NETVDevice::BaseDeviceFactory* getFactory();


    protected slots:

    ///Device selector slot, called when we change the device
    void comboActivated(int index);

    ///Configuration button clicked
    void configureButtonClicked();

    ///Device arguments changed
    void deviceArgsChanged(const QString &text);

    protected:

    ///The current factory
    NETVDevice::BaseDeviceFactory* m_factory;
	
};


#endif




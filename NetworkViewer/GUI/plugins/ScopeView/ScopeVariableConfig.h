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

#ifndef _SCOPEVARIABLECONFIG_H_
#define _SCOPEVARIABLECONFIG_H_

#include <QDialog>
#include "ui_ScopeVariableConfig.h"
#include <QColorDialog>
#include "ScopeCurve.h"
#include "ScopeView.h"

/**
    Simple configuration dialog to select color of the curve.
*/
class ScopeVariableConfig : public QDialog, public Ui::ScopeVariableConfig
{

	Q_OBJECT;

public:
        /**
            Constructor
            \param curve The curve to configure
            \param parent The parent Widget
        */
        ScopeVariableConfig(ScopeCurve *curve, ScopeView *parent = NULL);

protected slots:

        /**
            This happens when we click on the set color button.
        */
	void setColorClicked();

        /**
            Select the color for the curve
            \param color the selected color
        */
	void colorSelected ( const QColor & color);


        /**
        */
        void removeVariableClicked();

protected:

        ///Color selection dialog
	QColorDialog *m_colorDialog;

        ///Related curve
	ScopeCurve *m_curve;

        ///Related view
        ScopeView *m_view;
};


#endif /* _SCOPEVARIABLECONFIG_H_ */

/**
     Copyright (C) 2009-2010 IntRoLab
     http://introlab.gel.usherbrooke.ca
     Dominic Letourneau, ing. M.Sc.A.
     Dominic.Letourneau@USherbrooke.ca
 */

#ifndef _SCOPEVARIABLECONFIG_H_
#define _SCOPEVARIABLECONFIG_H_

#include <QDialog>
#include "ui_ScopeVariableConfig.h"
#include <QColorDialog>
#include "ScopeCurve.h"

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
	ScopeVariableConfig(ScopeCurve *curve, QWidget *parent = NULL);

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

protected:

        ///Color selection dialog
	QColorDialog *m_colorDialog;

        ///Related curve
	ScopeCurve *m_curve;
};


#endif /* _SCOPEVARIABLECONFIG_H_ */

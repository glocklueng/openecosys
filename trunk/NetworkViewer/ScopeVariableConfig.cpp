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
#include "ScopeVariableConfig.h"


ScopeVariableConfig::ScopeVariableConfig(ScopeCurve *curve, QWidget *parent)
	: QDialog(parent), m_curve(curve)
{
	setupUi(this);

	//Set Button color
	QPen pen = curve->pen();

	m_label->setAutoFillBackground(true);
	QPalette palette = m_label->palette();
	palette.setColor( QPalette::Window, pen.color() );
	palette.setColor(QPalette::WindowText, pen.color());
	m_label->setPalette(palette);

	//Set Color Dialog
	connect(m_colorButton, SIGNAL(clicked()),this,SLOT(setColorClicked()));

}
void ScopeVariableConfig::setColorClicked()
{
	QColorDialog dialog(this);

	connect(&dialog,SIGNAL(colorSelected(const QColor&)),this,SLOT(colorSelected(const QColor&)));

	dialog.exec();
}

void ScopeVariableConfig::colorSelected ( const QColor & color)
{
	qDebug("ColorSelected...");
	m_curve->setColor(color);

	//Update label
	m_label->setAutoFillBackground(true);
	QPalette palette = m_label->palette();
	palette.setColor( QPalette::Window, color );
	palette.setColor(QPalette::WindowText, color);
	m_label->setPalette(palette);

}

/**
     Copyright (C) 2009-2010 IntRoLab
     http://introlab.gel.usherbrooke.ca
     Dominic Letourneau, ing. M.Sc.A.
     Dominic.Letourneau@USherbrooke.ca
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

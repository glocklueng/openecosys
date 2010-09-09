/**
     Copyright (C) 2009-2010 IntRoLab
     http://introlab.gel.usherbrooke.ca
     Dominic Letourneau, ing. M.Sc.A.
     Dominic.Letourneau@USherbrooke.ca
 */

#include "ScopeCurve.h"

ScopeCurve::ScopeCurve(ModuleVariable *var, QwtPlot *parentPlot)
:	QwtPlotCurve("Untitled"),m_variable(var),  m_plot(parentPlot)
{
	Q_ASSERT(m_variable);

	//Attach the curve the the plot
	attach(parentPlot);
	
	//Connect signals for change
	connect(m_variable, SIGNAL(valueChanged(ModuleVariable*)), this, SLOT(updateVariable(ModuleVariable*)));
	
	//Update the current variable value
	updateVariable(m_variable);
}

ScopeCurve::~ScopeCurve()
{
	detach();
}

void ScopeCurve::updateVariable(ModuleVariable *var)
{
	if (var)
	{
		//Get current time
		double elapsed = ScopeCurve::elapsed();
		
		
		//Get current value (to double)
		bool ok = false;
		double value = var->getValue().toDouble(&ok);
		
		if (ok)
		{
			//Push back values
			if (m_time.size() < SCOPE_CURVE_MAX_SIZE)
			{
				m_time.push_back(elapsed);
				m_values.push_back(value);
			}
			else 
			{
				
				m_time.push_back(elapsed);
				m_values.push_back(value);
				
				//Resize data
				m_time.erase(m_time.begin());
				m_values.erase(m_values.begin());
			}
			
			setData(&m_time[0], &m_values[0], m_time.size());
			
			m_plot->replot();
		}
	}
}


double ScopeCurve::elapsed()
{
	static QTime creationTime = QTime::currentTime();
	static bool initialized = false;
	if (!initialized)
	{
		creationTime.start();
		initialized = true;
	}
	
	return (double) creationTime.elapsed() / 1000.0;
}

void ScopeCurve::setColor(const QColor &color)
{
	setPen(QPen(color));
	m_plot->replot();
}

ModuleVariable* ScopeCurve::getVariable()
{
	return m_variable;
}

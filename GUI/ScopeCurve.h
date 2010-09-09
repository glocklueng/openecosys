/**
     Copyright (C) 2009-2010 IntRoLab
     http://introlab.gel.usherbrooke.ca
     Dominic Letourneau, ing. M.Sc.A.
     Dominic.Letourneau@USherbrooke.ca
 */

#ifndef _SCOPE_CURVE_H_
#define _SCOPE_CURVE_H_

#include <QHBoxLayout>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include "ui_ScopeView.h"
#include "ModuleConfiguration.h"
#include <vector>
#include <QTime>
#include <QTimer>

///Maximum of points on the curve for each variable
#define SCOPE_CURVE_MAX_SIZE 2000


/**
    A scope curve based on the QwtPlotCorve from Qwt toolkit
    \author Dominic Letourneau
*/
class ScopeCurve : public QObject, public QwtPlotCurve
{
	Q_OBJECT;
	
public:
	
        /**
            Constructor
            \param var The variable to plot. When the valueChanged is emitted, a new plot point will be generated.
            \param parentPlot The plot where to draw the curve
        */
	ScopeCurve(ModuleVariable *var, QwtPlot *parentPlot);
	
        ///Destructor
        virtual ~ScopeCurve();

        /**
            \return ModuleVariable* The variable that is associated with the ScopeCurve
        */
	ModuleVariable* getVariable();

public slots:
	
	/**
                This will be used to update the plot, this will be event based...
                \param var The vairable updated (should be == internal ModuleVariable*)
	 */
	void updateVariable(ModuleVariable *var);
	
	/**
            Making sure every plot starts from the same elapsed time...
            \return double the time from the start of the application
        */
	static double elapsed();

        /**
            Set the curve color
            \param color The Curve color
        */
	void setColor(const QColor &color);
	
protected:
	
        ///The variable that is being plotted
	ModuleVariable *m_variable;

        ///Time values for the plot (x-axis)
	std::vector<double> m_time;

        ///Variable values for the plot (y-axis)
	std::vector<double> m_values;

        ///The plot where to draw the curve
	QwtPlot *m_plot;
};


#endif


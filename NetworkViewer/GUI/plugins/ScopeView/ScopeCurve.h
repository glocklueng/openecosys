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
#ifndef _SCOPE_CURVE_H_
#define _SCOPE_CURVE_H_

#include <QHBoxLayout>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_series_data.h>
#include "ui_ScopeView.h"
#include "ModuleConfiguration.h"
#include <QVector>
#include <QDateTime>
#include <QTimer>

///Maximum of points on the curve for each variable
#define SCOPE_CURVE_DEFAULT_BUFFER_SIZE 500


class ScopeCurveData : public QwtSeriesData<QPointF>
{
public:

    ScopeCurveData();

    //Reimplemented from QwtSeriesData
    virtual QPointF sample( size_t i ) const;
    virtual size_t size() const;
    virtual QRectF boundingRect() const;
    void setMaxItems(size_t count);
    int getMaxItems();

    void append(const QPointF &sample);
    void clear();

protected:

    void updateBoundaries();

    ///Time values for the plot (x-axis)
    QVector<float> m_time;

    ///Variable values for the plot (y-axis)
    QVector<float> m_values;

    QRectF m_boundingRect;

    size_t m_maxItems;

};



/**
    A scope curve based on the QwtPlotCurve from Qwt toolkit
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
    static double elapsed(const QDateTime& time);

    /**
    Set the curve color
    \param color The Curve color
    */
    void setColor(const QColor &color);

    /**
    Set the maximum size of the data
    \param size the length of the buffers
    */
    void setMaximumBufferSize(unsigned long size);

    /**
    Clear the buffer (empty the data)
    */
    void clearBuffer();


signals:

    void aboutToDestroy(ScopeCurve *ptr);

protected slots:
	
protected:
	


    ///The variable that is being plotted
    ModuleVariable *m_variable;

    ///The data
    ScopeCurveData *m_data;

    ///The plot where to draw the curve
    QwtPlot *m_plot;

    ///The maximum buffer size
    unsigned long m_maxBufferSize;


};


#endif


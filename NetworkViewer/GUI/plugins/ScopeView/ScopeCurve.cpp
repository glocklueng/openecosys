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

#include "ScopeCurve.h"
#include "qwt_symbol.h"
#include "qwt_legend.h"
#include "qwt_legend_item.h"
#include <math.h>
#include <algorithm>

ScopeCurveData::ScopeCurveData()
 : m_boundingRect(1.0, 1.0, -2.0, -2.0) //invalid
{

}

QPointF ScopeCurveData::sample( size_t i ) const
{
    if (i < m_time.size())
    {
        return QPointF(m_time[i],m_values[i]);
    }
    else
    {
        qDebug("Out of bound : %i",i);
    }
}

size_t ScopeCurveData::size() const
{
    return m_time.size();
}

QRectF ScopeCurveData::boundingRect() const
{
    return m_boundingRect;
}

void ScopeCurveData::append(const QPointF &sample)
{
    if (m_boundingRect.width() < 0 || m_boundingRect.height() < 0 )
    {
        m_boundingRect.setRect( sample.x(), sample.y(), 0.0, 0.0 );
    }
    else
    {
        m_boundingRect.setRight( sample.x() );

        if ( sample.y() > m_boundingRect.bottom() )
            m_boundingRect.setBottom( sample.y() );

        if ( sample.y() < m_boundingRect.top() )
            m_boundingRect.setTop( sample.y() );
    }

    //Push data
    m_time.append(sample.x());
    m_values.append(sample.y());

}

ScopeCurve::ScopeCurve(ModuleVariable *var, QwtPlot *parentPlot)
    :	m_variable(var),  m_plot(parentPlot), m_maxBufferSize(SCOPE_CURVE_DEFAULT_BUFFER_SIZE)
{
    Q_ASSERT(m_variable);

    m_data = new ScopeCurveData();

    setTitle(m_variable->getName() + " [" + QString::number(m_variable->getDeviceID()) + "]");


    //Attach the curve the the plot
    attach(parentPlot);

    //Set Style
    //setStyle(QwtPlotCurve::Dots);


    //Connect signals for change
    connect(m_variable, SIGNAL(valueChanged(ModuleVariable*)), this, SLOT(updateVariable(ModuleVariable*)));

    //Update the current variable value
    updateVariable(m_variable);


    //Set data holder
    setData(m_data);
}

ScopeCurve::~ScopeCurve()
{
    emit aboutToDestroy(this);
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

            m_data->append(QPointF(elapsed,value));

            m_plot->replot();
/*
            //Push back values
            if (m_time.size() < m_maxBufferSize)
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

            //setData(&m_time[0], &m_values[0], m_time.size());
            setSamples(m_time,m_values);


            //draw curve (all)
            //draw(std::max(0,m_values.size() - 2) ,m_values.size() -1);
            //m_plot->replot();
 */
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


    //setSymbol(QwtSymbol(QwtSymbol::Ellipse,brush(),pen(),QSize(3,3)));
    //

    m_plot->replot();
}

ModuleVariable* ScopeCurve::getVariable()
{
    return m_variable;
}


void ScopeCurve::setMaximumBufferSize(unsigned long size)
{
    /*

    m_maxBufferSize = size;

    if (m_time.size() > m_maxBufferSize)
    {
        //Resize data, keep last m_maxBufferSize;
        m_time.erase(m_time.begin(),m_time.begin() + (m_time.size() - m_maxBufferSize));
        m_values.erase(m_values.begin(),m_values.begin() + (m_values.size() - m_maxBufferSize));
    }

    //set plot data
    //setData(&m_time[0], &m_values[0], m_time.size());
    setSamples(m_time,m_values);

    */

}

void ScopeCurve::clearBuffer()
{
    /*

    m_time.resize(0);
    m_values.resize(0);

    //set plot data
    //setData(&m_time[0], &m_values[0], m_time.size());
    setSamples(m_time,m_values);

    */

}

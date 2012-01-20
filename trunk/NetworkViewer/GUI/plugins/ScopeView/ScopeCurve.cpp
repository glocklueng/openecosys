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
    : m_boundingRect(1.0, 1.0, -2.0, -2.0), m_maxItems(SCOPE_CURVE_DEFAULT_BUFFER_SIZE)
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
    Q_ASSERT(m_time.size() == m_values.size());

    //Push data
    m_time.append(sample.x());
    m_values.append(sample.y());

    if (m_time.size() < m_maxItems)
    {
        //Initial Setup
        if (m_boundingRect.width() < 0 || m_boundingRect.height() < 0 )
        {
            m_boundingRect.setRect( sample.x(), sample.y(), 0.0, 0.0 );
        }
        else
        {
            //Update if necessary
            m_boundingRect.setRight( sample.x() );

            if ( sample.y() > m_boundingRect.bottom() )
                m_boundingRect.setBottom( sample.y() );

            if ( sample.y() < m_boundingRect.top() )
                m_boundingRect.setTop( sample.y() );
        }
    }
    else
    {
        //Must remove one element first
        m_time.remove(0,m_time.size() - m_maxItems);
        m_values.remove(0,m_values.size() - m_maxItems);

        //Update boundaries
        updateBoundaries();
    }



}

void ScopeCurveData::setMaxItems(size_t count)
{
    m_maxItems = count;

    //Resize data vector (if too big)
    if (m_time.size() > count)
    {
        m_time.remove(0,m_time.size() - m_maxItems);
        m_values.remove(0,m_values.size() - m_maxItems);
    }

    //Nothing to do when too small...


    updateBoundaries();
}

int ScopeCurveData::getMaxItems()
{
    return m_maxItems;
}


void ScopeCurveData::updateBoundaries()
{
    Q_ASSERT(m_time.size() == m_values.size());

    //Will scan all data for boundaries...
    if (m_time.empty())
    {
        //Back to "illegal" size
        m_boundingRect.setRect(1.0, 1.0, -2.0, -2.0);
    }
    else
    {
        //initalize to first element
        m_boundingRect.setTop(m_values[0]);
        m_boundingRect.setBottom(m_values[0]);

        //Values are ordered in time, we already know the bounding time box
        m_boundingRect.setLeft(m_time[0]);
        m_boundingRect.setRight(m_time.last());

        //Scan the array and adjust boudaries if necessary
        for (unsigned int i = 0; i < m_values.size(); i++)
        {
            //Lowe value test
            if ( m_values[i] > m_boundingRect.bottom())
            {
                m_boundingRect.setBottom( m_values[i] );
            }

            //Upper value test
            if ( m_values[i] < m_boundingRect.top())
            {
                    m_boundingRect.setTop( m_values[i] );
            }
        }
    }

}

void ScopeCurveData::clear()
{
    m_time.resize(0);
    m_values.resize(0);
    updateBoundaries();
}



ScopeCurve::ScopeCurve(ModuleVariable *var, QwtPlot *parentPlot)
    :	m_variable(var),  m_plot(parentPlot), m_maxBufferSize(SCOPE_CURVE_DEFAULT_BUFFER_SIZE), m_data(NULL)
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
    setSymbol(new QwtSymbol(QwtSymbol::Ellipse,brush(),pen(),QSize(3,3)));
    m_plot->replot();
}

ModuleVariable* ScopeCurve::getVariable()
{
    return m_variable;
}


void ScopeCurve::setMaximumBufferSize(unsigned long size)
{
    if (m_data)
    {
        m_data->setMaxItems(size);
    }

}

void ScopeCurve::clearBuffer()
{
    if (m_data)
    {
        m_data->clear();
    }
}


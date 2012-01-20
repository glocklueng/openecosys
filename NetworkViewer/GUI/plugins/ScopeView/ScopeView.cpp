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

#include "ScopeView.h"
#include <QTreeWidgetItem>
#include "ScopeVariableConfig.h"
#include <QMenu>
#include <QMessageBox>
#include "NetworkView.h"
#include <QDropEvent>
#include <QDragEnterEvent>
#include "qwt_symbol.h"
#include "qwt_plot_grid.h"
#include "qwt_legend.h"


//This will insert the plugin in the dictionary...
static int scope_view_plugin_init = BasePlugin::registerPlugin("ScopeView",new BasePlugin::PluginFactory<ScopeView>());

ScopeView::ScopeView(NetworkView *parent)
    : BasePlugin(parent), m_zoomer(NULL), m_picker(NULL)
{

    setupUi(this);

    setAcceptDrops(true);

    //Create (empty) Plot
    m_plot = new QwtPlot(this);

    //Plot background color & grid
    QwtPlotGrid *grid = new QwtPlotGrid;
    grid->setMajPen(QPen(Qt::gray, 0, Qt::DotLine));
    grid->attach(m_plot);
    m_plot->setCanvasBackground(QColor(29, 100, 141)); // nice blue


    m_frame->setLayout(new QVBoxLayout());
    m_frame->layout()->addWidget(m_plot);

    //Create magnifier for plot
    //m_zoomer = new QwtPlotZoomer(m_plot->canvas());
    //m_zoomer->setMaxStackDepth(10);
    //m_zoomer->setTrackerMode(QwtPicker::AlwaysOn);
    //m_zoomer->setZoomBase(QwtDoubleRect(0,0,0,0));
    //qDebug("Initial zoom index %i",m_zoomer->zoomRectIndex());

    m_picker = new QwtPlotPicker(m_plot->canvas());
    m_picker->setTrackerMode(QwtPicker::AlwaysOn);

    m_plot->setAutoReplot(true);

    m_updateTimer = new QTimer(this);
    connect(m_updateTimer,SIGNAL(timeout()),this,SLOT(updateTimer()));
    m_updateTimer->start(50);//50ms timer


    //Create Legend at bottom
    QwtLegend *legend = new QwtLegend(this);
    m_plot->insertLegend(legend,QwtPlot::BottomLegend);
    legend->setItemMode(QwtLegend::ClickableItem);
    connect(m_plot,SIGNAL(legendClicked(QwtPlotItem*)),this,SLOT(legendItemClicked(QwtPlotItem*)));


    //Connect tool buttons & spinbox
    connect(m_clearToolButton,SIGNAL(clicked()),this,SLOT(clearCurves()));
    connect(m_bufferSizeSpinbox,SIGNAL(valueChanged(int)),this,SLOT(setCurveMaxBufferSize(int)));



}


void ScopeView::legendItemClicked(QwtPlotItem *plotItem)
{
    qDebug("ScopeView::legendItemClicked(QwtPlotItem *plotItem = %p) rtti: %i",plotItem,plotItem->rtti());

    if (plotItem->rtti() == QwtPlotItem::Rtti_PlotCurve)
    {
        ScopeCurve *curve = dynamic_cast<ScopeCurve*>(plotItem);
        if (curve)
        {
             ScopeVariableConfig config(curve,this);
             config.exec();
        }
    }

}


void ScopeView::init()
{

}



void ScopeView::terminate()
{

}



void ScopeView::addCurve(ModuleVariable *variable)
{
    //Make sure we don't already have this curve...
    for( int i = 0; i < m_curves.size(); i++)
    {
        if (m_curves[i]->getVariable() == variable)
        {
            return;
        }
    }

    ScopeCurve *curve = new ScopeCurve(variable,m_plot);

    connect(curve,SIGNAL(aboutToDestroy(ScopeCurve*)),this,SLOT(destroyCurve(ScopeCurve*)));


    switch (m_curves.size() % 10)
    {
    case 0:
        curve->setColor(Qt::red);
        break;

    case 1:
        curve->setColor(Qt::darkYellow);
        break;

    case 2:
        curve->setColor(Qt::green);
        break;

    case 3:
        curve->setColor(Qt::blue);
        break;

    case 4:
        curve->setColor(Qt::cyan);
        break;

    case 5:
        curve->setColor(Qt::magenta);
        break;

    case 6:
        curve->setColor(Qt::darkCyan);
        break;

    case 7:
        curve->setColor(Qt::darkGreen);
        break;

    case 8:
        curve->setColor(Qt::darkRed);
        break;

    case 9:
        curve->setColor(Qt::yellow);
        break;

    default:
        curve->setColor(Qt::black);
        break;

    }

    m_curves.push_back(curve);

    connect(variable,SIGNAL(aboutToDestroy(ModuleVariable*)),this,SLOT(removeCurve(ModuleVariable*)));
}

void ScopeView::removeCurve(ModuleVariable *variable)
{
    qDebug("ScopeView::removeCurve(ModuleVariable *variable = %p)",variable);

    for (int i = 0; i < m_curves.size(); i++)
    {
        //Need to remove curve for destroyed module
        if (m_curves[i]->getVariable() == variable)
        {
            ScopeCurve *curve = m_curves[i];
            //Remove Curve from the list
            m_curves.removeAll(curve);
            curve->deleteLater();
            break;
        }
    }
}

void ScopeView::updateTimer()
{
    m_plot->replot();
}




void ScopeView::destroyCurve(ScopeCurve *curve)
{
   qDebug("ScopeView::destroyCurve(ScopeCurve *curve = %p)",curve);
   m_curves.removeAll(curve);
}


bool ScopeView::event ( QEvent * e )
{
    if(e->type() == BasePluginEvent::eventType())
    {

        BasePluginEvent *event = dynamic_cast<BasePluginEvent*>(e);

        if(event)
        {
            //Get the message...
            for(QMap<QString,QVariant>::iterator iter = event->m_map.begin(); iter != event->m_map.end(); iter++)
            {
                if (iter.key() == "addCurve")
                {
                    bool ok;
                    ModuleVariable *var = (ModuleVariable*) iter.value().toLongLong(&ok);
                    if (ok && var)
                    {
                        addCurve(var);
                    }
                }
            }
        }

        e->accept();
        return true;
    }

    return BasePlugin::event(e);
}

void ScopeView::dropEvent(QDropEvent *dropEvent)
{
    qDebug("ScopeView::dropEvent(QDragEnterEvent *event)");
    qDebug()<< dropEvent->mimeData()->text();
    qDebug()<< dropEvent->mimeData()->formats();


    QByteArray data = dropEvent->mimeData()->data("application/x-qabstractitemmodeldatalist");

    //qDebug("data size : %i",data.size());
    QDataStream stream(&data, QIODevice::ReadOnly);

    while (!stream.atEnd())
    {
        int row, col;
        QMap<int,  QVariant> roleDataMap;
        stream >> row >> col >> roleDataMap;

        //qDebug("got something from row: %i, col: %i",row,col);
        if (roleDataMap.contains(Qt::UserRole))
        {
            //qDebug() << "Found : " << roleDataMap[Qt::UserRole];
            ModuleVariable *variable = (ModuleVariable*) roleDataMap[Qt::UserRole].toULongLong();

            //qDebug("got pointer : %p",variable);
            Q_ASSERT(variable != NULL);
            addCurve(variable);
        }
    }
}


void ScopeView::dragEnterEvent(QDragEnterEvent *event)
{
    qDebug("ScopeView::dragEnterEvent(QDragEnterEvent *event)");
    qDebug()<< event->mimeData()->text();
    qDebug()<< event->mimeData()->formats();

    if (event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist"))
    {
        event->acceptProposedAction();
    }
}

void ScopeView::setCurveMaxBufferSize(int size)
{

    qDebug("ScopeView::setCurveMaxBufferSize(int size = %i)",size);

    //Set all curve to the new buffer size
    for(unsigned int i = 0; i<m_curves.size(); i++)
    {
        m_curves[i]->setMaximumBufferSize(size);
    }

    //Replot
    m_plot->replot();
}


void ScopeView::clearCurves()
{
    qDebug("ScopeView::clearCurves()");

    //Set all curve to the new buffer size
    for(unsigned int i = 0; i<m_curves.size(); i++)
    {
        m_curves[i]->clearBuffer();
    }

    //Replot
    m_plot->replot();
}




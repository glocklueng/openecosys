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
#include "qwt_symbol.h"
#include "qwt_plot_grid.h"
#include "qwt_legend.h"


//This will insert the plugin in the dictionary...
static int scope_view_plugin_init = BasePlugin::registerPlugin("ScopeView",new BasePlugin::PluginFactory<ScopeView>());

bool ScopeView::eventFilter(QObject *obj, QEvent *event)
{    
    if (event->type() == QEvent::Drop)
    {
        //qDebug("Drop event on the Tree widget");

        QDropEvent *dropEvent = dynamic_cast<QDropEvent*>(event);
        //qDebug("ScopeView::eventFilter(QDropEvent *event = %p)",dropEvent);

        if (dropEvent)
        {

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
        return true;
    }

   // standard event processing
   return QObject::eventFilter(obj, event);

}


ScopeView::ScopeView(NetworkView *parent)
    : BasePlugin(parent), m_zoomer(NULL), m_picker(NULL)
{

    setupUi(this);
    setAcceptDrops(true);



    /*

    //Filter events on the tree widget
    m_treeWidget->setAcceptDrops(true);
    m_treeWidget->setDragDropMode(QAbstractItemView::DropOnly);
    m_treeWidget->installEventFilter(this);
    m_treeWidget->viewport()->installEventFilter(this);

    */

    //Create (empty) Plot
    m_plot = new QwtPlot(m_frame);


    m_plot->setAcceptDrops(true);
    m_plot->installEventFilter(this);
    m_plot->canvas()->setAcceptDrops(true);
    m_plot->canvas()->installEventFilter(this);
    m_frame->setAcceptDrops(true);
    m_frame->installEventFilter(this);



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

/*
    m_treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    for (unsigned int i = 0; i < TREE_WIDGET_COLUMN_SIZE; i++)
    {
        m_treeWidget->resizeColumnToContents(i);
    }



    connect(m_treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(moduleItemDoubleClicked(QTreeWidgetItem*,int)));
    connect(m_treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(moduleItemClicked(QTreeWidgetItem*,int)));
    //m_treeWidget->grabKeyboard();
*/


    m_plot->setAutoReplot(true);

    m_updateTimer = new QTimer(this);
    connect(m_updateTimer,SIGNAL(timeout()),this,SLOT(updateTimer()));
    //m_updateTimer->start(100);//10ms timer


    //Create Legend at bottom
    QwtLegend *legend = new QwtLegend(this);
    m_plot->insertLegend(legend,QwtPlot::BottomLegend);
    legend->setItemMode(QwtLegend::ClickableItem);
    connect(m_plot,SIGNAL(legendClicked(QwtPlotItem*)),this,SLOT(legendItemClicked(QwtPlotItem*)));



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

    /*
    QTreeWidgetItem *item = new QTreeWidgetItem(m_treeWidget);
    QPen pen = curve->pen();
    item->setBackground(COLOR_COLUMN,pen.color());
    item->setIcon(NAME_COLUMN,QIcon(QPixmap(":images/dsPIC.png")));
    item->setText(NAME_COLUMN,variable->getName() + "[" + QString::number(variable->getDeviceID()) + "]");
    item->setText(ACTION_COLUMN,"[Remove]");
    item->setIcon(ACTION_COLUMN,QIcon(QPixmap(":images/cross.png")));
    m_itemCurveMap.insert(item,curve);

    //Make sure everything is visible
    for (unsigned int i = 0; i < TREE_WIDGET_COLUMN_SIZE; i++)
    {
        m_treeWidget->resizeColumnToContents(i);
    }
    */
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

            /*
            for (QMap<QTreeWidgetItem *, ScopeCurve*>::iterator iter = m_itemCurveMap.begin();
            iter != m_itemCurveMap.end(); iter++)
            {
                if (iter.value() == curve)
                {
                    QTreeWidgetItem * item = iter.key();

                    //Let's delete the item and the curve
                    //m_itemCurveMap.remove(item);
                    m_curves.removeAll(curve);

                    delete curve;
                    delete item;

                    //Make sure everything is visible
                    for (unsigned int i = 0; i < TREE_WIDGET_COLUMN_SIZE; i++)
                    {
                        m_treeWidget->resizeColumnToContents(i);
                    }
                    break;

                }
            }
            */


            break;
        }
    }
}


/*
void ScopeView::moduleItemDoubleClicked ( QTreeWidgetItem * item, int column )
{
    qDebug("ScopeView::moduleItemClicked ( QTreeWidgetItem * item %p, int column %i)",item,column);

    ScopeVariableConfig config(m_itemCurveMap[item],this);

    config.exec();

    ScopeCurve *curve = m_itemCurveMap[item];

    QPen pen = curve->pen();

    item->setBackground(COLOR_COLUMN,pen.color());

}

void ScopeView::moduleItemClicked(QTreeWidgetItem * item, int column)
{
    //Test for remove action
    if (column == ACTION_COLUMN)
    {
        QMessageBox msgBox;
        msgBox.setText(QString("Do you want to remove the variable : ") + item->text(0) + " ?");
        msgBox.setInformativeText(QString("This will remove the variable from the scope view."));
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Cancel);
        int ret = msgBox.exec();

        if (ret == QMessageBox::Ok)
        {
            ScopeCurve* curve = m_itemCurveMap[item];
            m_itemCurveMap.remove(item);

            if (curve)
            {
                m_curves.removeAll(curve);
                delete curve;
            }

            if (item)
            {
                delete item;
            }
        }
    }

}
*/

void ScopeView::customContextMenuRequested ( const QPoint & pos )
{
    qDebug("Custom menu requested");

    QMenu* menu = new QMenu(this);
    menu->setTitle(tr("SubNetwork"));

    menu->addAction( new QAction("Hello World!", this) );
    menu->addAction( new QAction("This Rocks!", this) );

    QAction *action = menu->exec(pos);

    if (action)
    {

    }

    delete menu;

}

void ScopeView::updateTimer()
{

    //m_plot->replot();

  //m_plot->replot();
/*
    for(int i =0; i < m_curves.size(); i++)
    {
        ModuleVariable *variable = m_curves[i]->getVariable();
        if (variable->getActivated())
        {
            m_view->requestVariable(variable);
        }
    }
*/

/*
    if (m_zoomer)
    {
        QwtDoubleRect rect;

        for (unsigned int i = 0; i < m_curves.size(); i++)
        {
            QwtDoubleRect curveRect = m_curves[i]->boundingRect();

            if (i == 0)
            {
                rect = curveRect;
            }
            else
            {
                rect.unite(curveRect);
            }
        }

        //m_zoomer->setZoomBase(rect);
        //qDebug("zoomRectIndex : %i",m_zoomer->zoomRectIndex());

        //Zoom at zoom base?
        if (m_zoomer->zoomRectIndex() <= 1)
        {
            //zoom base too large?
            if (m_zoomer->zoomBase() != rect)
            {
                //qDebug("Zoom rect index == 0");
                //qDebug("canvas Replot requested, zoom base different");
                //m_zoomer->setZoomBase(rect);
                //m_zoomer->zoom(0);
                //qDebug("zoomRectIndex : %i",m_zoomer->zoomRectIndex());
            }
        }

    }
 */

}

void ScopeView::dropEvent(QDropEvent *event)
{
    qDebug("ScopeView::dropEvent(QDropEvent *event)");
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

void ScopeView::destroyCurve(ScopeCurve *curve)
{
   qDebug("ScopeView::destroyCurve(ScopeCurve *curve = %p)",curve);
   m_curves.removeAll(curve);
}

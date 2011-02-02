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

    //Filter events on the tree widget
    m_treeWidget->setAcceptDrops(true);
    m_treeWidget->setDragDropMode(QAbstractItemView::DropOnly);
    m_treeWidget->installEventFilter(this);
    m_treeWidget->viewport()->installEventFilter(this);

    //Create (empty) Plot
    m_plot = new QwtPlot(m_frame);
    m_frame->setLayout(new QVBoxLayout());
    m_frame->layout()->addWidget(m_plot);

    //Create magnifier for plot
    //m_zoomer = new QwtPlotZoomer(m_plot->canvas());
    //m_picker = new QwtPlotPicker(m_plot->canvas());
    //m_picker->setTrackerMode(QwtPicker::AlwaysOn);


    m_treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);


    connect(m_treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(moduleItemDoubleClicked(QTreeWidgetItem*,int)));
    connect(m_treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(moduleItemClicked(QTreeWidgetItem*,int)));
    //m_treeWidget->grabKeyboard();

    m_updateTimer = new QTimer(this);
    connect(m_updateTimer,SIGNAL(timeout()),this,SLOT(updateTimer()));
    m_updateTimer->start(10);//10ms timer
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

    //Add curve symbol
    curve->setSymbol(QwtSymbol(QwtSymbol::Diamond,curve->brush(),curve->pen(),QSize(10,10)));


    m_curves.push_back(curve);

    connect(variable,SIGNAL(aboutToDestroy(ModuleVariable*)),this,SLOT(removeCurve(ModuleVariable*)));

    QTreeWidgetItem *item = new QTreeWidgetItem(m_treeWidget);

    QPen pen = curve->pen();
    item->setBackground(COLOR_COLUMN,pen.color());

    item->setIcon(NAME_COLUMN,QIcon(QPixmap(":images/dsPIC.png")));
    item->setText(NAME_COLUMN,variable->getName() + "[module:" + QString::number(variable->getDeviceID()) + "]");

    item->setText(ACTION_COLUMN,"[Remove]");
    item->setIcon(ACTION_COLUMN,QIcon(QPixmap(":images/cross.png")));

    m_itemCurveMap.insert(item,curve);
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

            for (QMap<QTreeWidgetItem *, ScopeCurve*>::iterator iter = m_itemCurveMap.begin();
            iter != m_itemCurveMap.end(); iter++)
            {
                if (iter.value() == curve)
                {
                    QTreeWidgetItem * item = iter.key();

                    //Let's delete the item and the curve
                    m_itemCurveMap.remove(item);
                    m_curves.removeAll(curve);

                    delete curve;
                    delete item;

                    break;

                }
            }
            break;
        }
    }
}


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

    for(int i =0; i < m_curves.size(); i++)
    {
        ModuleVariable *variable = m_curves[i]->getVariable();
        if (variable->getActivated())
        {
            m_view->requestVariable(variable);
        }
    }
}

void ScopeView::dropEvent(QDropEvent *event)
{
    qDebug("ScopeView::dropEvent(QDropEvent *event)");
}



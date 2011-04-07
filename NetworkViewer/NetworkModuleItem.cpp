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

#include "NetworkModuleItem.h"
#include <QBuffer>
#include <QGraphicsSceneMouseEvent>
#include <QFileDialog>
#include <QBrush>
#include "NETV_define.h"


CrossModuleItem::CrossModuleItem(NetworkModuleItem *parent)
    :  QGraphicsPixmapItem(QPixmap(":images/cross.png"), parent), m_networkModuleItem(parent)
{

}

void CrossModuleItem::mousePressEvent ( QGraphicsSceneMouseEvent * event )
{
    if (event->button() & Qt::LeftButton)
    {
        //qDebug() << "CrossModuleItem pos : " << event->pos();
        m_networkModuleItem->removeModule();
    }
}


NetworkModuleItem::NetworkModuleItem(NetworkModule* module, QGraphicsItem *parent)
: QGraphicsPixmapItem(QPixmap(":images/dsPIC.png"), parent), m_module(module), m_textItem(NULL), m_rectItem(NULL), m_crossItem(NULL)
{
	QBuffer buffer;
	buffer.open(QIODevice::ReadWrite);
	
	ModuleConfiguration *conf = module->getConfiguration();
	Q_ASSERT(conf);
	
	conf->prettyPrint(buffer);
	
	int device_id = conf->getDeviceID();
	
	
	QRectF bounds = boundingRect();
	
	//Add labels under
	m_textItem = new QGraphicsTextItem(this);
	QFont font = m_textItem->font();
	font.setPixelSize(50);
	m_textItem->setFont(font);
	
	m_textItem->setPlainText(QString(" [ ")
					  + QString::number((unsigned int) conf->getDeviceID())
					  + QString(" ]")
					  );
	QRectF textBounds = m_textItem->boundingRect();	
	
        //Center-align Text
	m_textItem->setPos(bounds.width() /2 - textBounds.width() / 2,bounds.height() + 10);
	

        //CROSS ITEM
        m_crossItem = new CrossModuleItem(this);
        m_crossItem->setPos(-50,-50);
        m_crossItem->scale(0.5,0.5);
        m_crossItem->setZValue(1);
        m_crossItem->hide();

	
	//Add status rectangle
	m_rectItem = new QGraphicsRectItem(0,0,bounds.width(),textBounds.height(), this);
	m_rectItem->setPos(0,bounds.height() + 10);
	
	
        QBrush myBrush(Qt::SolidPattern);
     
        myBrush.setColor(Qt::blue);

	switch(conf->getModuleState())
	{
                case NETV_IDLE_MODE_ID :
			myBrush.setColor(Qt::yellow);
		break;
		
                case NETV_BOOT_MODE_ID:
			myBrush.setColor(Qt::red);
		break;
		
                case NETV_NORMAL_MODE_ID:
			myBrush.setColor(Qt::green);
		break;    

		default:
			myBrush.setColor(Qt::yellow);
		break;
	}   
	m_rectItem->setBrush(myBrush);
        m_rectItem->setZValue(0);
        m_textItem->setZValue(1);

	
	//Add tooltip
	setToolTip(QString(buffer.data()));
	
	
	scale(0.25,0.25);
}

NetworkModule* NetworkModuleItem::getNetworkModule()
{
	return m_module;
}

void NetworkModuleItem::mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event )
{
	
	//qDebug("NetworkModuleItem::mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event )");
	emit doubleClicked(this);
}

void NetworkModuleItem::mousePressEvent ( QGraphicsSceneMouseEvent * event )
{
	//qDebug("NetworkModuleItem::mousePressEvent ( QGraphicsSceneMouseEvent * event )");
	

        if (event->button() & Qt::RightButton)
	{

            //Menu deroulant.

	}

}

void NetworkModuleItem::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
{
	//qDebug("NetworkModuleItem::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )");
}

void NetworkModuleItem::timeUpdate(QTime time)
{
	m_lastUpdate = time;
}

QTime NetworkModuleItem::getLastUpdateTime()
{
	return m_lastUpdate;
}

void NetworkModuleItem::setActive(bool active)
{
    if(m_module)
    {
        m_module->setActive(active);
    }

    if (m_crossItem)
    {
        if (active)
        {
            m_crossItem->hide();
        }
        else
        {
            m_crossItem->show();
        }
    }
}

void NetworkModuleItem::removeModule()
{
    emit removeModule(this);
}

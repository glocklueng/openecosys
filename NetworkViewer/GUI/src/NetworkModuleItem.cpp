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
#include <QDir>
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
: QGraphicsPixmapItem(parent), m_module(module), m_textItem(NULL), m_rectItem(NULL), m_crossItem(NULL)
{
        QBuffer myBuffer;
        myBuffer.open(QIODevice::ReadWrite);
	
        ModuleConfiguration *conf = m_module->getConfiguration();
	Q_ASSERT(conf);
	

        //Will change color when state change
        connect(conf,SIGNAL(moduleStateChanged()),this,SLOT(stateChanged()));


        conf->prettyPrint(myBuffer);
	
	int device_id = conf->getDeviceID();

        //We have an arduino device here
        if (conf->getProcessorID() == 0x3412)
        {
            this->setPixmap(QPixmap(":images/Arduino.png"));
        }
	else
	{
	    setPixmap(QPixmap(":images/dsPIC.png")); 
	}

	
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
	
        //Config text item
        m_configTextItem = new QGraphicsTextItem(m_textItem);
        m_configTextItem->setFont(font);
        m_configTextItem->setPlainText(conf->getConfigName());
        QRectF configBounds = m_configTextItem->boundingRect();
        m_configTextItem->setPos(textBounds.width() / 2 - configBounds.width() / 2,textBounds.height() + 10);



        //CROSS ITEM
        m_crossItem = new CrossModuleItem(this);
        m_crossItem->setPos(-50,-50);
        m_crossItem->scale(0.5,0.5);
        m_crossItem->setZValue(1);

        if (m_module->active())
        {
            m_crossItem->hide();
        }

	
	//Add status rectangle
        m_rectItem = new QGraphicsRectItem(0,0,bounds.width(),textBounds.height(), this);
        m_rectItem->setPos(0,bounds.height() + 10);
		
        QBrush myBrush(Qt::SolidPattern);

	switch(conf->getModuleState())
	{		
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
        setToolTip(QString(myBuffer.data()));
	
	scale(0.35,0.35);
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


void NetworkModuleItem::stateChanged()
{
    ModuleConfiguration *conf = m_module->getConfiguration();
    Q_ASSERT(conf);
    if (m_rectItem)
    {
        QBrush myBrush(Qt::SolidPattern);

        switch(conf->getModuleState())
        {
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
    }
}

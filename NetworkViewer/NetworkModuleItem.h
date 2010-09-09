/**
     Copyright (C) 2009-2010 IntRoLab
     http://introlab.gel.usherbrooke.ca
     Dominic Letourneau, ing. M.Sc.A.
     Dominic.Letourneau@USherbrooke.ca
 */

#ifndef _NETWORK_MODULE_ITEM_H_
#define _NETWORK_MODULE_ITEM_H_

#include "NetworkModule.h"
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QGraphicsRectItem>
#include <QObject>
#include <QTime>

class NetworkModuleItem;

/**
    When a module is inactive, we will display a cross to remove the module from the list
    \author Dominic Letourneau
*/
class CrossModuleItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT;

public:

    /**
        Constructor
        \param parent The parent \ref NetworkModuleItem
    */
    CrossModuleItem(NetworkModuleItem *parent);

protected:

    ///The parent NetworkModuleItem
    NetworkModuleItem *m_networkModuleItem;

    ///Mouse event, we are interested at mouse clicks basically
    virtual void mousePressEvent ( QGraphicsSceneMouseEvent * event );

};

/**
    Base module display object
    \author Dominic Letourneau
*/
class NetworkModuleItem : public QObject, public QGraphicsPixmapItem
{
	Q_OBJECT;
	
public:
	
        /**
            Constructor
            \param module The module we are displaying information from
            \param parent The parent item, default = NULL
        */
	NetworkModuleItem(NetworkModule* module, QGraphicsItem *parent = NULL);	

        /**
            \return NetworkModule* The associated network module
        */
	NetworkModule* getNetworkModule();

        /**
            \return QTime The last update time. We are polling for alive modules on the bus. This is the last time the module answered the "ALIVE" request.
        */
	QTime getLastUpdateTime();

        /**
            A module is set "active" when it is "ALIVE". If the module stops responding for few seconds, it will be put inactive.
            \param alive The alive flag
        */
        void setActive(bool active);

        /**
            Will emit the removeModule signal
        */
        void removeModule();
	
public slots:

        /**
            Update the last update time
            \param time the current time
        */
	void timeUpdate(QTime time);

signals:
	
        /**
            Emit this signal when the module icon is double clicked
            \param item Mostly == this
        */
        void doubleClicked(NetworkModuleItem* item);

        /**
            Emit this signal when the module needs to be removed
            \param item Mostly == this
        */
        void removeModule(NetworkModuleItem* item);
	
	
protected:
	
        ///Double click event
	virtual void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event );	
        ///Mouse press event
	virtual void mousePressEvent ( QGraphicsSceneMouseEvent * event );	
        ///Mouse release event
	virtual void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
	
        ///The related module
	NetworkModule *m_module;
        ///Last time it has been updated (seen on the bus)
	QTime m_lastUpdate;
        ///Text information
	QGraphicsTextItem *m_textItem;
        ///Rectangle surrounding the icon
	QGraphicsRectItem *m_rectItem;
        ///Cross icon to remove the module when inactive
        CrossModuleItem *m_crossItem;
};


#endif

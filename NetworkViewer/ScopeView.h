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

#ifndef _SCOPE_VIEW_H_
#define _SCOPE_VIEW_H_


#include "ScopeCurve.h"
#include "BasePlugin.h"

class NetworkView;
class ScopeView;

/*

class TreeViewEventFilter : public QObject
{
    Q_OBJECT;

public:

    TreeViewEventFilter(ScopeView *view);

    protected:

    bool eventFilter(QObject *obj, QEvent *event);

    ScopeView *m_view;

};
*/

/**
    Scope main window
    \author Dominic Letourneau
*/
class ScopeView : public BasePlugin, public Ui::ScopeView
{

    Q_OBJECT;

public:

    /**
        Constructor
        \param parent Parent widget, default = NULL
    */
    ScopeView(NetworkView *parent= NULL);

    /**
     * 	NOT YET IMPLEMENTED, BUT PLUGINS NEED TO OVERLOAD THIS FUNCTION
     *
     */
    virtual void init();


    /**
     *  NOT YET IMPLEMENTED, BUT PLUGINS NEED TO OVERLOAD THIS FUNCTION
     *
     */
    virtual void terminate();

public slots:


    /**
        Add a curve to the scope view
        \param variable the variable to plot
    */
    void addCurve(ModuleVariable *variable);


    /**
        Remove a curve from the scope view
        \param  variable to remove
    */
    void removeCurve(ModuleVariable *variable);

protected slots:

    /**
        \todo Not yet implemented.
    */
    void customContextMenuRequested ( const QPoint & pos );

    /**
        Module double clicked
        \param item The item clicked
        \param column The column clicked
    */
    void moduleItemDoubleClicked ( QTreeWidgetItem * item, int column );

    /**
        Module clicked
        \param item The item clicked
        \param column The column clicked
    */
    void moduleItemClicked(QTreeWidgetItem * item, int column);


    ///Internal drop event handler
    virtual void dropEvent(QDropEvent *event);

    void updateTimer();


protected:

    bool eventFilter(QObject *obj, QEvent *event);

    ///The plot
    QwtPlot *m_plot;

    ///List of all curves on the plot
    QList<ScopeCurve*> m_curves;

    ///Mapping of tree view items and scope curves
    QMap<QTreeWidgetItem *, ScopeCurve*> m_itemCurveMap;

    QTimer *m_updateTimer;

};


#endif

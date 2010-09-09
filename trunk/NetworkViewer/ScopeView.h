/**
Copyright (C) 2009-2010 IntRoLab
http://introlab.gel.usherbrooke.ca
Dominic Letourneau, ing. M.Sc.A.
Dominic.Letourneau@USherbrooke.ca
*/

#ifndef _SCOPE_VIEW_H_
#define _SCOPE_VIEW_H_


#include "ScopeCurve.h"

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
class ScopeView : public QWidget, public Ui::ScopeView
{

    Q_OBJECT;

public:

    /**
        Constructor
        \param parent Parent widget, default = NULL
    */
    ScopeView(NetworkView *parent= NULL);

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

    ///The network view
    NetworkView *m_view;

    ///The plot
    QwtPlot *m_plot;

    ///List of all curves on the plot
    QList<ScopeCurve*> m_curves;

    ///Mapping of tree view items and scope curves
    QMap<QTreeWidgetItem *, ScopeCurve*> m_itemCurveMap;

    QTimer *m_updateTimer;

};


#endif

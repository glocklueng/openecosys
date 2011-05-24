#include "ScopePlot.h"
#include "ScopeView.h"
#include <QtDebug>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QMap>

ScopePlot::ScopePlot(ScopeView *parent)
    : QwtPlot(parent), m_view(parent)
{
    //Will accept drop by default
    //setAcceptDrops(true);
}

/*

void ScopePlot::dropEvent(QDropEvent *event)
{
    qDebug("ScopePlot::dropEvent(QDropEvent *event = %p)",event);
    qDebug()<< event->mimeData()->text();
    qDebug()<< event->mimeData()->formats();

    QByteArray data = event->mimeData()->data("application/x-qabstractitemmodeldatalist");

    qDebug("data size : %i",data.size());
    QDataStream stream(&data, QIODevice::ReadOnly);

    while (!stream.atEnd())
    {
        int row, col;
        QMap<int,  QVariant> roleDataMap;
        stream >> row >> col >> roleDataMap;

        qDebug("got something from row: %i, col: %i",row,col);

        if (roleDataMap.contains(Qt::UserRole))
        {
            qDebug() << "Found : " << roleDataMap[Qt::UserRole];

            ModuleVariable *variable = (ModuleVariable*) roleDataMap[Qt::UserRole].toULongLong();

            qDebug("got pointer : %p",variable);

            if (m_view)
            {
                m_view->addCurve(variable);
            }
        }

    }

}

void ScopePlot::dragEnterEvent(QDragEnterEvent *event)
{

    qDebug("ScopePlot::dragEnterEvent(QDragEnterEvent *event)");
    qDebug()<< event->mimeData()->text();
    qDebug()<< event->mimeData()->formats();

    if (event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist"))
    {
        event->acceptProposedAction();
    }
}

*/

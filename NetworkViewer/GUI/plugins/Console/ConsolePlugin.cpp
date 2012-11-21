/**
     Copyright (C) 2009-2012 IntRoLab
     http://introlab.3it.usherbrooke.ca
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

#include "ConsolePlugin.h"
#include "NetworkView.h"

#include "NETVMessageEvent.h"
#include <QTextEdit>

//This will insert the plugin in the dictionary...
static int console_plugin_init = BasePlugin::registerPlugin("ConsolePlugin",new BasePlugin::PluginFactory<ConsolePlugin>());



ConsolePlugin::ConsolePlugin(NetworkView *view)
	 : BasePlugin(view)
{
    qDebug("ConsolePlugin::ConsolePlugin(NetworkView *view = %p)",view);

    QList<NETVInterfaceManager*> managers = m_view->getInterfaceManagerList();

    //Register to all interface
    for (int i = 0; i<managers.size(); i++)
    {
        NETVInterfaceHandler *handler = managers[i]->getInterfaceHandler();

        if (handler)
        {
            handler->registerObserver(this);
        }
    }

    //Connect add /remove managers
    connect(m_view,SIGNAL(interfaceAdded(NETVInterfaceManager*)),this,SLOT(interfaceAdded(NETVInterfaceManager*)));
    connect(m_view,SIGNAL(interfaceRemoved(NETVInterfaceManager*)),this,SLOT(interfaceRemoved(NETVInterfaceManager*)));

	//Setup UI
	m_ui.setupUi(this);

    //Signals
    connect(m_ui.m_saveButton,SIGNAL(clicked()),this,SLOT(saveButtonClicked()));
    connect(m_ui.m_clearButton,SIGNAL(clicked()),this,SLOT(clearButtonClicked()));


}

ConsolePlugin::~ConsolePlugin()
{
    QList<NETVInterfaceManager*> managers = m_view->getInterfaceManagerList();

    //Unregister to all interface
    for (int i = 0; i<managers.size(); i++)
    {
        NETVInterfaceHandler *handler = managers[i]->getInterfaceHandler();

        if (handler)
        {
            handler->unregisterObserver(this);
        }
    }
}

void ConsolePlugin::init()
{

}

void ConsolePlugin::terminate()
{

}

void ConsolePlugin::notifyNETVMessage(const NETV_MESSAGE &msg)
{

    //WATCH OUT, THIS IS CALLED FROM ANOTHER THREAD (NETVInterfaceHandler working thread)
    //Posting message to self (in the GUI thread)
    NETVMessageEvent *event = new NETVMessageEvent(msg);
    QCoreApplication::postEvent (this, event, Qt::HighEventPriority);

}

bool  ConsolePlugin::event ( QEvent * e )
{
    if (e->type() == QEvent::User)
    {
        //qDebug("NETVInterfaceManager::event %p QEvent::User",e);
        if (NETVMessageEvent *event = dynamic_cast<NETVMessageEvent*>(e))
        {
            processMessage(event->getMessage());
            return true;
        }
    }
    return BasePlugin::event(e);
}

int ConsolePlugin::createTab(int id)
{
    bool found = false;
    int index = 0;
    //Verify if tab exist
    for (int i =0; i < m_ui.m_tabWidget->count(); i++)
    {
        if (m_ui.m_tabWidget->tabText(i) == QString::number(id))
        {
            index = i;
            found = true;
            break;
        }
    }

    if (!found)
    {
        QTextEdit *widget = new QTextEdit(m_ui.m_tabWidget);

        //int	addTab ( QWidget * page, const QIcon & icon, const QString & label )
        index = m_ui.m_tabWidget->addTab(widget,QString::number(id));
    }

    return index;
}



void ConsolePlugin::processMessage(const NETV_MESSAGE &msg)
{
    //Look for console messages
    if (msg.msg_type == NETV_TYPE_EVENTS &&
            msg.msg_cmd == NETV_EVENTS_CMD_CONSOLE)
    {
        int index = createTab(msg.msg_dest);

        QTextEdit *widget = dynamic_cast<QTextEdit*>(m_ui.m_tabWidget->widget(index));

        if (widget)
        {
            QCursor c = widget->cursor();
            widget->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
            widget->insertPlainText(QByteArray((char*)msg.msg_data,(int)msg.msg_data_length));
            widget->setCursor(c);
        }
    }
}

void ConsolePlugin::clearButtonClicked()
{
    int index = m_ui.m_tabWidget->currentIndex();

    if (index == 0)
    {
        m_ui.m_globalTextEdit->clear();

    }
    else
    {
        QTextEdit *widget = dynamic_cast<QTextEdit*>(m_ui.m_tabWidget->widget(index));

        if (widget)
        {
            widget->clear();
        }
        else
        {
            qDebug("No widget at index : %i",index);
        }
    }
}

void ConsolePlugin::saveButtonClicked()
{
    qDebug("ConsolePlugin::saveButtonClicked() - Not yet implemted");
}

void ConsolePlugin::interfaceAdded(NETVInterfaceManager *manager)
{
    manager->getInterfaceHandler()->registerObserver(this);
}

void ConsolePlugin::interfaceRemoved(NETVInterfaceManager *manager)
{
    manager->getInterfaceHandler()->unregisterObserver(this);
}

#include "NETVInterfaceManagerView.h"
#include "NetworkView.h"
#include "DeviceSelectorDialog.h"
#include "UserPreferences.h"



NETVInterfaceManagerView::NETVInterfaceManagerView(NetworkView *parent, bool loadPrefs)
    : QDialog(parent), m_view(parent)
{
    setupUi(this);

    //Signals
    connect(m_addButton,SIGNAL(clicked()),this,SLOT(addButtonClicked()));
    connect(m_removeButton,SIGNAL(clicked()),this,SLOT(removeButtonClicked()));

    if (loadPrefs)
    {
        //NetworkViewer Startup will try to load saved prefs
        loadUserPrefs();
    }
    else
    {
        //Already started, create list from already existing interfaces
        createInterfaceList();
    }
}


void NETVInterfaceManagerView::addButtonClicked()
{
    qDebug("NETVInterfaceManagerView::addButtonClicked()");

    DeviceSelectorDialog dialog(this);

    if (dialog.exec())
    {
        //Let's add the interface
        if (dialog.getFactory())
        {

            NETVDevice *dev = NULL;

            qDebug("Found factory");
            if(dialog.args().size())
            {
                dev = NETVDevice::createDevice(dialog.selectedDevice(), dialog.args().toStdString().c_str());
            }
            else
            {
                dev = NETVDevice::createDevice(dialog.selectedDevice(),NULL);
            }

            if (dev)
            {

                NETVInterfaceManager *manager = new NETVInterfaceManager(dev,NULL,m_view);

                if (m_view->addNETVInterfaceManager(manager))
                {
                    QListWidgetItem *item = new QListWidgetItem(dialog.selectedDevice() + " Args : [" + dialog.args() + "]",m_listWidget);
                    m_listWidget->addItem(item);
                    m_itemMap.insert(item,manager);
                }
            }
        }

        saveUserPrefs();
    }
}

void NETVInterfaceManagerView::removeButtonClicked()
{
    qDebug("NETVInterfaceManagerView::removeButtonClicked()");

    QListWidgetItem* item = m_listWidget->currentItem();

    if (item && m_itemMap.contains(item))
    {
        NETVInterfaceManager *manager = m_itemMap[item];
        m_view->removeNETVInterfaceManager(manager);
        m_listWidget->takeItem(m_listWidget->currentRow());
        m_itemMap.remove(item);
        delete item;
    }

    saveUserPrefs();
}


void NETVInterfaceManagerView::createInterfaceList()
{
    QList<NETVInterfaceManager*> managers = m_view->getInterfaceManagerList();

    for (unsigned int i = 0; i < managers.size(); i++)
    {
        NETVInterfaceHandler *handler = managers[i]->getInterfaceHandler();
        NETVDevice *dev = handler->getDevice();

        //Get the name & args
        QString name = dev->getName();
        QString args = dev->getArgs();

        //Insert in the list
        QListWidgetItem *item = new QListWidgetItem(name + " Args : [" + args + "]",m_listWidget);
        m_listWidget->addItem(item);
        m_itemMap.insert(item,managers[i]);
    }


}

void NETVInterfaceManagerView::saveUserPrefs()
{
    QList<NETVInterfaceManager*> managers = m_view->getInterfaceManagerList();


    UserPreferences& prefs = UserPreferences::getGlobalPreferences();

    prefs.setKey("NETVInterfaceManagerView:nb_interface",QString::number(managers.size()),false);


    for (unsigned int i = 0; i < managers.size(); i++)
    {
        NETVInterfaceHandler *handler = managers[i]->getInterfaceHandler();
        NETVDevice *dev = handler->getDevice();

        //Get the name & args
        QString name = dev->getName();
        QString args = dev->getArgs();

        prefs.setKey("NETVInterfaceManagerView:name_" + QString::number(i),name,false);
        prefs.setKey("NETVInterfaceManagerView:args_" + QString::number(i),args,false);

    }


    prefs.save();
}

void NETVInterfaceManagerView::loadUserPrefs()
{
    UserPreferences& prefs = UserPreferences::getGlobalPreferences();

    if (prefs.contains("NETVInterfaceManagerView:nb_interface"))
    {
        int count = prefs.getKey("NETVInterfaceManagerView:nb_interface").toInt();

        //Let's create the interfaces...
        for (unsigned int i = 0; i < count; i++)
        {
            QString name_key = QString("NETVInterfaceManagerView:name_") + QString::number(i);
            QString args_key = QString("NETVInterfaceManagerView:args_") + QString::number(i);

            //Let's create the interface
            if (prefs.contains(name_key) && prefs.contains(args_key))
            {
                QString name = prefs.getKey(name_key).toString();
                QString args = prefs.getKey(args_key).toString();

                NETVDevice *dev = NULL;


                if(args.size())
                {
                    dev = NETVDevice::createDevice(name, args.toStdString().c_str());
                }
                else
                {
                    dev = NETVDevice::createDevice(name,NULL);
                }

                if (dev)
                {

                    NETVInterfaceManager *manager = new NETVInterfaceManager(dev,NULL,m_view);

                    if (m_view->addNETVInterfaceManager(manager))
                    {
                        QListWidgetItem *item = new QListWidgetItem(name + " Args : [" + args + "]",m_listWidget);
                        m_listWidget->addItem(item);
                        m_itemMap.insert(item,manager);
                    }
                }

            }
        }
    }
}


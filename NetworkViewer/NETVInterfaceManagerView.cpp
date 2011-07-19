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

    //Signals
    connect(m_listWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(itemDoubleClicked(QListWidgetItem*)));
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
                    int period = dialog.getUpdatePeriod();
                    QListWidgetItem *item = new QListWidgetItem(dialog.selectedDevice() + " Args : [" + dialog.args() + "]" + " Period (ms) :" + QString::number(period),m_listWidget);
                    m_listWidget->addItem(item);
                    m_itemMap.insert(item,manager);

                    //update scheduling
                    manager->getScheduler()->setVariableRequestInterval(period);
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
        NetworkScheduler *scheduler = managers[i]->getScheduler();

        int period = scheduler->getVariableRequestInterval();

        //Get the name & args
        QString name = dev->getName();
        QString args = dev->getArgs();

        //Insert in the list
        QListWidgetItem *item = new QListWidgetItem(name + " Args : [" + args + "]" + " Period (ms) :" + QString::number(period),m_listWidget);
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
        NetworkScheduler *scheduler = managers[i]->getScheduler();

        //Get the name & args
        QString name = dev->getName();
        QString args = dev->getArgs();

        prefs.setKey("NETVInterfaceManagerView:name_" + QString::number(i),name,false);
        prefs.setKey("NETVInterfaceManagerView:args_" + QString::number(i),args,false);
        prefs.setKey("NETVInterfaceManagerView:updatePeriod_" + QString::number(i),QString::number(scheduler->getVariableRequestInterval()),false);
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
            QString period_key = QString("NETVInterfaceManagerView:updatePeriod_") + QString::number(i);

            //Let's create the interface
            if (prefs.contains(name_key) && prefs.contains(args_key) && prefs.contains(period_key))
            {
                QString name = prefs.getKey(name_key).toString();
                QString args = prefs.getKey(args_key).toString();

                int period = prefs.getKey(period_key).toInt();

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
                        QListWidgetItem *item = new QListWidgetItem(name + " Args : [" + args + "]" + " Period (ms) :" + QString::number(period),m_listWidget);
                        m_listWidget->addItem(item);
                        m_itemMap.insert(item,manager);

                        //update scheduling
                        manager->getScheduler()->setVariableRequestInterval(period);

                    }
                }

            }
        }
    }
}

void NETVInterfaceManagerView::itemDoubleClicked ( QListWidgetItem * item )
{
    if (m_itemMap.contains(item))
    {
        NETVInterfaceManager *manager = m_itemMap[item];
        NETVInterfaceHandler* handler = manager->getInterfaceHandler();
        NetworkScheduler *scheduler = manager->getScheduler();
        NETVDevice *dev = handler->getDevice();

        //Get device name & args
        QString name = dev->getName();
        QString args = dev->getArgs();

        DeviceSelectorDialog dialog(this,name,args);
        dialog.setUpdatePeriod(scheduler->getVariableRequestInterval());

        //Set actual device & parameters
        if (dialog.exec())
        {
            scheduler->setVariableRequestInterval(dialog.getUpdatePeriod());
            saveUserPrefs();

            //Update item text
            item->setText(name + " Args : [" + args + "]" + " Period (ms) :" + QString::number(dialog.getUpdatePeriod()));
        }
    }
}


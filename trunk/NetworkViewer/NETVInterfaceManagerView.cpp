#include "NETVInterfaceManagerView.h"
#include "NetworkView.h"
#include "DeviceSelectorDialog.h"
#include "UserPreferences.h"



NETVInterfaceManagerView::NETVInterfaceManagerView(NetworkView *parent)
    : QDialog(parent), m_view(parent)
{
    setupUi(this);

    //Signals
    connect(m_addButton,SIGNAL(clicked()),this,SLOT(addButtonClicked()));
    connect(m_removeButton,SIGNAL(clicked()),this,SLOT(removeButtonClicked()));

    createInterfaceList();
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


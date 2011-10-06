#ifndef _NETVINTERFACEMANAGERVIEW_H
#define _NETVINTERFACEMANAGERVIEW_H

#include "ui_NETVInterfaceManagerView.h"
#include <QListWidgetItem>
#include "NETVInterfaceManager.h"
#include <QMap>

//forward declaration
class NetworkView;

class NETVInterfaceManagerView : public QDialog, public Ui::NETVInterfaceManagerView
{
    Q_OBJECT;

public:

    NETVInterfaceManagerView(NetworkView *parent = NULL, bool loadPrefs = false);


protected slots:

    void addButtonClicked();
    void removeButtonClicked();
    void itemDoubleClicked ( QListWidgetItem * item );

protected:

    void saveUserPrefs();
    void loadUserPrefs();
    void createInterfaceList();
    NetworkView *m_view;
    QMap<QListWidgetItem*, NETVInterfaceManager*> m_itemMap;
};

#endif // _NETVINTERFACEMANAGERVIEW_H

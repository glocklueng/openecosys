#ifndef _PREFERENCES_DIALOG_H_
#define _PREFERENCES_DIALOG_H_

#include "ui_PreferencesDialog.h"
#include <QDialog>

class PreferencesDialog : public QDialog, public Ui::PreferencesDialog
{
    Q_OBJECT

public:

    PreferencesDialog(QWidget *parent = NULL);



};


#endif

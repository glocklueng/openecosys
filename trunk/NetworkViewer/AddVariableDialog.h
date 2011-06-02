/**
     Copyright (C) 2009-2011 IntRoLab
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
#ifndef ADDVARIABLEDIALOG_H
#define ADDVARIABLEDIALOG_H

#include <QDialog>
#include "ModuleVariable.h"
#include "NetworkModule.h"

namespace Ui {
    class AddVariableDialog;
}

class AddVariableDialog : public QDialog
{
    Q_OBJECT

public:

    QString getVariableName();
    ModuleVariable::VARIABLE_TYPE getVariableType();
    ModuleVariable::VARIABLE_MEMORY_TYPE getVariableMemoryType();
    int getVariableMemoryOffset();
    QString getVariableDescription();

    explicit AddVariableDialog(NetworkModule *module = NULL);
    ~AddVariableDialog();

private:
    Ui::AddVariableDialog *ui;
    NetworkModule *m_module;
};

#endif // ADDVARIABLEDIALOG_H

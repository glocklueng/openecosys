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

#include "AddVariableDialog.h"
#include "ui_AddVariableDialog.h"

AddVariableDialog::AddVariableDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddVariableDialog)
{
    ui->setupUi(this);
}

AddVariableDialog::~AddVariableDialog()
{
    delete ui;
}

QString AddVariableDialog::getVariableName()
{
    return ui->m_variableNameLineEdit->text();
}

ModuleVariable::VARIABLE_TYPE AddVariableDialog::getVariableType()
{
    return (ModuleVariable::VARIABLE_TYPE) ui->m_variableTypeCombo->currentIndex();
}

ModuleVariable::VARIABLE_MEMORY_TYPE AddVariableDialog::getVariableMemoryType()
{
    return (ModuleVariable::VARIABLE_MEMORY_TYPE) ui->m_variableMemoryTypeCombo->currentIndex();
}

int AddVariableDialog::getVariableMemoryOffset()
{
    return ui->m_variableOffsetSpinBox->value();
}

QString AddVariableDialog::getVariableDescription()
{
    return ui->m_variableDescriptionTextEdit->toPlainText();
}

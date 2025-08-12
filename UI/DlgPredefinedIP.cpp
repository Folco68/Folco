/**************************************************************************************** 
 *                                                                                      * 
 *     Folco - Program allowing to quickly change the IPv4 address of an interface      * 
 *                       Copyright (C) 2024-2025 Martial Demolins                       * 
 *                                                                                      * 
 *         This program is free software: you can redistribute it and/or modify         * 
 *         it under the terms of the GNU General Public License as published by         * 
 *          the Free Software Foundation, either version 3 of the License, or           * 
 *                          (at your option) any later version                          * 
 *                                                                                      * 
 *            This program is distributed in the hope that it will be useful            * 
 *            but WITHOUT ANY WARRANTY; without even the implied warranty of            * 
 *            MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             * 
 *                     GNU General Public License for more details                      * 
 *                                                                                      * 
 *          You should have received a copy of the GNU General Public License           * 
 *         along with this program.  If not, see <https://www.gnu.org/licenses>         * 
 *                                                                                      * 
 ****************************************************************************************/

#include "DlgPredefinedIP.hpp"
#include "../Global.hpp"
#include "../Logger.hpp"
#include "ui_DlgPredefinedIP.h"
#include <QPalette>
#include <QStringList>

// Support null ptrs. Useless here, but it supports it anyway
DlgPredefinedIP::DlgPredefinedIP(QWidget* parent, QString* name, QString* ip, QString* networkmask, QString* gateway)
    : QDialog(parent)
    , ui(new Ui::DlgPredefinedIP)
    , Dialog(this)
{
    ui->setupUi(this);
    setWindowTitle(QString("%1 - Edit Predefined IP").arg(APPLICATION_NAME));

    // Fill the UI if arguments were provided
    if (name != nullptr) {
        ui->EditName->setText(*name);
    }

    if (ip != nullptr) {
        ui->EditIP->setText(*ip);
    }

    if (networkmask != nullptr) {
        ui->EditMask->setText(*networkmask);
    }

    if (gateway != nullptr) {
        ui->EditGateway->setText(*gateway);
    }

    // Connections
    connect(ui->ButtonOK, &QPushButton::clicked, this, [this]() { accept(); });
    connect(ui->ButtonCancel, &QPushButton::clicked, this, [this]() { reject(); });
    connect(ui->EditIP, &QLineEdit::textChanged, this, [this]() { updateButtonOK(); });
    connect(ui->EditMask, &QLineEdit::textChanged, this, [this]() { updateButtonOK(); });
    connect(ui->EditGateway, &QLineEdit::textChanged, this, [this]() { updateButtonOK(); });

    // Set OK button according to IPs
    updateButtonOK();
}

DlgPredefinedIP::~DlgPredefinedIP()
{
    delete ui;
}

void DlgPredefinedIP::updateButtonOK()
{
    ui->ButtonOK->setEnabled((ui->EditIP->hasAcceptableInput() && !ui->EditIP->text().isEmpty()) && ui->EditMask->hasAcceptableInput()
                             && ui->EditGateway->hasAcceptableInput());
}

int DlgPredefinedIP::newPredefinedIP(QWidget* parent, QString* name, QString* ip, QString* networkmask, QString* gateway)
{
    int RetVal;

    DlgPredefinedIP* Dlg = new DlgPredefinedIP(parent);
    Dlg->setWindowTitle("New predefined IP");
    RetVal = Dlg->exec();

    if (RetVal == QDialog::Accepted) {
        *name        = Dlg->ui->EditName->text();
        *ip          = Dlg->ui->EditIP->text();
        *networkmask = Dlg->ui->EditMask->text();
        *gateway     = Dlg->ui->EditGateway->text();
        Logger::instance()->addLogEntry(QString("Predefined IP created. Name: %1. IP: %2. Mask: %3. Gateway: %4").arg(*name, *ip, *networkmask, *gateway));
    }

    delete Dlg;
    return RetVal;
}

int DlgPredefinedIP::editPredefinedIP(QWidget* parent, QString* name, QString* ip, QString* networkmask, QString* gateway)
{
    int RetVal;

    DlgPredefinedIP* Dlg = new DlgPredefinedIP(parent, name, ip, networkmask, gateway);
    Dlg->setWindowTitle("Edit predefined IP");
    RetVal = Dlg->exec();

    if (RetVal == QDialog::Accepted) {
        Logger::instance()->addLogEntry(QString("Modifying Predifined IP: Name: %1. IP: %2. Mask: %3. Gateway: %4").arg(*name, *ip, *networkmask, *gateway));
        *name        = Dlg->ui->EditName->text();
        *ip          = Dlg->ui->EditIP->text();
        *networkmask = Dlg->ui->EditMask->text();
        *gateway     = Dlg->ui->EditGateway->text();
        Logger::instance()->addLogEntry(QString("Predifined IP modified. Name: %1. IP: %2. Mask: %3. Gateway: %4").arg(*name, *ip, *networkmask, *gateway));
    }

    delete Dlg;
    return RetVal;
}

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

DlgPredefinedIP::DlgPredefinedIP(QString title)
    : ui(new Ui::DlgPredefinedIP)
    , Dialog(this)
{
    ui->setupUi(this);
    setWindowTitle(QString("%1 - %2").arg(APPLICATION_NAME, title == nullptr ? "New Predefined IP" : title));

    // Connections
    connect(ui->ButtonOK, &QPushButton::clicked, this, [this]() { accept(); });
    connect(ui->ButtonCancel, &QPushButton::clicked, this, [this]() { reject(); });
    connect(ui->EditIP, &QLineEdit::textChanged, this, [this]() { updateButtonOK(); });
    connect(ui->EditMask, &QLineEdit::textChanged, this, [this]() { updateButtonOK(); });
    connect(ui->EditGateway, &QLineEdit::textChanged, this, [this]() { updateButtonOK(); });

    // Set OK button according to IP validity
    updateButtonOK();
}

DlgPredefinedIP::DlgPredefinedIP(QString* name, QString* ip, QString* networkmask, QString* gateway)
    : DlgPredefinedIP("Edit Predefined IP")
{
        ui->EditName->setText(*name);
        ui->EditIP->setText(*ip);
        ui->EditMask->setText(*networkmask);
        ui->EditGateway->setText(*gateway);
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

int DlgPredefinedIP::newPredefinedIP(QString* name, QString* ip, QString* networkmask, QString* gateway)
{
    int RetVal;

    DlgPredefinedIP* Dlg = new DlgPredefinedIP;
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

int DlgPredefinedIP::editPredefinedIP(QString* name, QString* ip, QString* networkmask, QString* gateway)
{
    int RetVal;

    DlgPredefinedIP* Dlg = new DlgPredefinedIP(name, ip, networkmask, gateway);
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

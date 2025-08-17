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

#include "DlgSettings.hpp"
#include "../Global.hpp"
#include "../Settings.hpp"
#include "ui_DlgSettings.h"

DlgSettings::DlgSettings()
    : ui(new Ui::DlgSettings)
    , Dialog(this)
{
    // Setup UI
    ui->setupUi(this);
    setWindowModality(Qt::NonModal);
    setWindowTitle(QString("%1 - Settings").arg(APPLICATION_NAME));
    adjustSize();

    // Set the UI according to the settings
    ui->CheckShowOnlyEthernetWiFi->setChecked(Settings::instance()->showOnlyEthernetWifi());
    ui->CheckShowOnlyUp->setChecked(Settings::instance()->showOnlyUp());
    ui->CheckShowOnlyPredefined->setChecked(Settings::instance()->showOnlyPredefined());

    // Connections
    connect(ui->ButtonOK, &QPushButton::clicked, this, [this]() { accept(); });
    connect(ui->ButtonCancel, &QPushButton::clicked, this, [this]() { reject(); });
}

DlgSettings::~DlgSettings()
{
    delete ui;
}

void DlgSettings::execDlgSettings()
{

    DlgSettings* Dlg = new DlgSettings();
    if (Dlg->exec() == QDialog::Accepted) {
        Settings::instance()->setShowOnlyEthernetWifi(Dlg->ui->CheckShowOnlyEthernetWiFi->isChecked());
        Settings::instance()->setShowOnlyUp(Dlg->ui->CheckShowOnlyUp->isChecked());
        Settings::instance()->setShowOnlyPredefined(Dlg->ui->CheckShowOnlyPredefined->isChecked());
        Settings::instance()->sync();
    }
    delete Dlg;
}

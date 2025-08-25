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

#include "DlgConfiguration.hpp"
#include "../Global.hpp"
#include "../Logger.hpp"
#include "../Network/ConfigurationList.hpp"
#include "../Network/PredefinedIP.hpp"
#include "DlgPredefinedIP.hpp"
#include "ui_DlgConfiguration.h"
#include <QAbstractSocket>
#include <QFile>
#include <QHostAddress>
#include <QList>
#include <QMessageBox>
#include <QNetworkAddressEntry>
#include <QPushButton>
#include <QTableWidgetItem>
#include <QWindow>

// This dialog deals only with QString for editing a PredefinedIP, so that the Interface is not modified if the dialog is cancelled

DlgConfiguration::DlgConfiguration(QNetworkInterface NetworkInterface)
    : DlgConfiguration(ConfigurationList::instance()->configuration(NetworkInterface.hardwareAddress()))
{
    // Fill the Interface properties field
    ui->EditName->setText(NetworkInterface.humanReadableName());
    ui->EditHWaddress->setText(NetworkInterface.hardwareAddress());

    // Display an IPv4 address if one is available
    QList<QNetworkAddressEntry> AddressEntries = NetworkInterface.addressEntries();
    for (int i = 0; i < AddressEntries.size(); i++) {
        QHostAddress IPaddress   = AddressEntries.at(i).ip();
        QHostAddress NetworkMask = AddressEntries.at(i).netmask();
        if (IPaddress.protocol() == QAbstractSocket::IPv4Protocol) {
            ui->EditIP->setText(QString("%1 / %2").arg(IPaddress.toString(), NetworkMask.toString()));
        }
    }
}

DlgConfiguration::DlgConfiguration(Configuration* configuration)
    : ui(new Ui::DlgConfiguration)
    , Dialog(this)
{
    ui->setupUi(this);
    setWindowTitle(QString("%1 - Network Interface Configuration").arg(APPLICATION_NAME));

    // Interface is null if no PredefinedIP has been defined for this device yet
    if (configuration != nullptr) {
        ui->ButtonForgetConfiguration->setVisible(true);

        // Name and HWaddress are already displayed if this Configuration is attached to a Newtork Interface
        // Not a big deal though, it's harmless
        ui->EditName->setText(configuration->humanReadableName());
        ui->EditCustomName->setText(configuration->customName());
        ui->EditHWaddress->setText(configuration->hardwareAddress());

        // Fill the PredefinedIP table
        QList<PredefinedIP*> IPlist = configuration->predefinedIPlist();
        int                  Count  = IPlist.size();
        ui->TablePredefinedIP->setRowCount(Count);

        for (int i = 0; i < Count; i++) {
            PredefinedIP* ip = IPlist.at(i);

            QTableWidgetItem* ItemName        = new QTableWidgetItem(ip->name());
            QTableWidgetItem* ItemIPaddress   = new QTableWidgetItem(ip->ipAddress());
            QTableWidgetItem* ItemNetworkMask = new QTableWidgetItem(ip->networkMask());
            QTableWidgetItem* ItemGateway     = new QTableWidgetItem(ip->gateway());

            ui->TablePredefinedIP->setItem(i, COLUMN_NAME, ItemName);
            ui->TablePredefinedIP->setItem(i, COLUMN_IP_ADDRESS, ItemIPaddress);
            ui->TablePredefinedIP->setItem(i, COLUMN_NETWORK_MASK, ItemNetworkMask);
            ui->TablePredefinedIP->setItem(i, COLUMN_GATEWAY, ItemGateway);
        }
    }

    // Dialog connections
    connect(ui->ButtonOK, &QPushButton::clicked, this, [this]() { accept(); });
    connect(ui->ButtonCancel, &QPushButton::clicked, this, [this]() { reject(); });
    connect(ui->ButtonForgetConfiguration, &QPushButton::clicked, this, [this, configuration]() { forgetConfiguration(configuration); });

    // Table connections
    connect(ui->ButtoNewIP, &QPushButton::clicked, this, [this]() { newPredefinedIP(); });
    connect(ui->TablePredefinedIP, &QTableWidget::itemSelectionChanged, this, [this]() { tableSelectionChanged(); });
    connect(ui->TablePredefinedIP, &QTableWidget::doubleClicked, this, [this]() { editPredefinedIP(); });
    connect(ui->ButtonEditIP, &QPushButton::clicked, this, [this]() { editPredefinedIP(); });
    connect(ui->ButtonDeleteIP, &QPushButton::clicked, this, [this]() { deletePredefinedIP(); });
    connect(ui->ButtonUp, &QPushButton::clicked, this, [this]() { moveUp(); });
    connect(ui->ButtonDown, &QPushButton::clicked, this, [this]() { moveDown(); });
}

DlgConfiguration::~DlgConfiguration()
{
    delete ui;
}

void DlgConfiguration::tableSelectionChanged()
{
    QList<QTableWidgetItem*> SelectedItems = ui->TablePredefinedIP->selectedItems();

    // Defaut: no selection, buttons are disabled
    bool Selected = false;
    bool AtTop    = false;
    bool AtBottom = false;

    // Enable some of them if there is a valid selection
    if (SelectedItems.size() != 0) {
        Selected = true;
        int Row  = SelectedItems.at(0)->row();
        AtTop    = (Row == 0);
        AtBottom = (Row == ui->TablePredefinedIP->rowCount() - 1);
    }

    // Set buttons
    ui->ButtonEditIP->setEnabled(Selected);
    ui->ButtonDeleteIP->setEnabled(Selected);
    ui->ButtonUp->setEnabled(Selected && !AtTop);
    ui->ButtonDown->setEnabled(Selected && !AtBottom);
}

void DlgConfiguration::execDlgConfiguration(QNetworkInterface NetworkInterface)
{
    DlgConfiguration* Dlg = new DlgConfiguration(NetworkInterface);
    if (Dlg->exec() == QDialog::Accepted) {
        //
        // If the dialog is validated:
        // - if the configuration does not exists yet and there is something to save
        //      - create the configuration
        //      - write its new content
        //      - add the configuration to the list
        // - if the configuration already exists:
        //      - remove its content
        //      - write its new content
        //
        Configuration* Configuration = ConfigurationList::instance()->configuration(NetworkInterface.hardwareAddress());

        if (Configuration == nullptr) {
            if ((Dlg->ui->TablePredefinedIP->rowCount() != 0) || !Dlg->ui->EditCustomName->text().isEmpty()) {
                Configuration
                    = new class Configuration(NetworkInterface.hardwareAddress(), Dlg->ui->EditCustomName->text(), NetworkInterface.humanReadableName());
                Dlg->writeContent(Configuration);
                ConfigurationList::instance()->addConfiguration(Configuration);
            }
            // Nothing to do if we validate an interface which does not exist yet and which has no content to store into
        }
        else {
            Configuration->clearContent();
            Dlg->writeContent(Configuration);
        }
    }
    delete Dlg;
}

void DlgConfiguration::execDlgConfiguration(Configuration* configuration)
{
    DlgConfiguration* Dlg = new DlgConfiguration(configuration);
    // We are sure that the interface exists
    if (Dlg->exec() == QDialog::Accepted) {
        configuration->clearContent();
        Dlg->writeContent(configuration);
    }
    delete Dlg;
}

void DlgConfiguration::writeContent(Configuration* configuration)
{
    Logger::instance()->addLogEntry(QString("Setting Predefined IP for Interface %1 (%2)").arg(configuration->hardwareAddress(), ui->EditCustomName->text()));

    // Write Configuration custom name
    configuration->setCustomName(ui->EditCustomName->text());

    // Write Configuration PredefinedIP
    for (int i = 0; i < ui->TablePredefinedIP->rowCount(); i++) {
        PredefinedIP* ip = new PredefinedIP(ui->TablePredefinedIP->item(i, COLUMN_NAME)->text(),
                                            ui->TablePredefinedIP->item(i, COLUMN_IP_ADDRESS)->text(),
                                            ui->TablePredefinedIP->item(i, COLUMN_NETWORK_MASK)->text(),
                                            ui->TablePredefinedIP->item(i, COLUMN_GATEWAY)->text());
        configuration->addPredefinedIP(ip);
        Logger::instance()->addLogEntry(QString("Name: %1. IP: %2. Mask: %3. Gateway: %4").arg(ip->name(), ip->ipAddress(), ip->networkMask(), ip->gateway()));
    }
}

void DlgConfiguration::newPredefinedIP()
{
    QString Name;
    QString IP;
    QString NetworkMask;
    QString Gateway;

    if (DlgPredefinedIP::newPredefinedIP(&Name, &IP, &NetworkMask, &Gateway) == QDialog::Accepted) {
        int Count = ui->TablePredefinedIP->rowCount();
        ui->TablePredefinedIP->setRowCount(Count + 1);

        QTableWidgetItem* ItemName        = new QTableWidgetItem(Name);
        QTableWidgetItem* ItemIP          = new QTableWidgetItem(IP);
        QTableWidgetItem* ItemNetworkMask = new QTableWidgetItem(NetworkMask);
        QTableWidgetItem* ItemGateway     = new QTableWidgetItem(Gateway);

        ui->TablePredefinedIP->setItem(Count, COLUMN_NAME, ItemName);
        ui->TablePredefinedIP->setItem(Count, COLUMN_IP_ADDRESS, ItemIP);
        ui->TablePredefinedIP->setItem(Count, COLUMN_NETWORK_MASK, ItemNetworkMask);
        ui->TablePredefinedIP->setItem(Count, COLUMN_GATEWAY, ItemGateway);
    }
}

void DlgConfiguration::editPredefinedIP()
{
    // The Edit button is enabled only if there are selected items, so SelectedItems is not empty
    QList<QTableWidgetItem*> SelectedItems = ui->TablePredefinedIP->selectedItems();
    int                      Row           = ui->TablePredefinedIP->row(SelectedItems.at(0));

    QString Name(ui->TablePredefinedIP->item(Row, COLUMN_NAME)->text());
    QString IP(ui->TablePredefinedIP->item(Row, COLUMN_IP_ADDRESS)->text());
    QString NetworkMask(ui->TablePredefinedIP->item(Row, COLUMN_NETWORK_MASK)->text());
    QString Gateway(ui->TablePredefinedIP->item(Row, COLUMN_GATEWAY)->text());

    if (DlgPredefinedIP::editPredefinedIP(&Name, &IP, &NetworkMask, &Gateway) == QDialog::Accepted) {
        ui->TablePredefinedIP->item(Row, COLUMN_NAME)->setText(Name);
        ui->TablePredefinedIP->item(Row, COLUMN_IP_ADDRESS)->setText(IP);
        ui->TablePredefinedIP->item(Row, COLUMN_NETWORK_MASK)->setText(NetworkMask);
        ui->TablePredefinedIP->item(Row, COLUMN_GATEWAY)->setText(Gateway);
    }
}

void DlgConfiguration::deletePredefinedIP()
{
    // The Delete button is enabled only if there are selected items, so SelectedItems is not empty
    QList<QTableWidgetItem*> SelectedItems = ui->TablePredefinedIP->selectedItems();
    int                      Row           = ui->TablePredefinedIP->row(SelectedItems.at(0));
    ui->TablePredefinedIP->removeRow(Row);
}

void DlgConfiguration::moveUp()
{
    QList<QTableWidgetItem*> SelectedItems = ui->TablePredefinedIP->selectedItems();
    int                      Row           = ui->TablePredefinedIP->row(SelectedItems.at(0));
    int                      ColumnCount   = ui->TablePredefinedIP->columnCount();

    for (int i = 0; i < ColumnCount; i++) {
        QTableWidgetItem* TopItem    = ui->TablePredefinedIP->item(Row - 1, i);
        QTableWidgetItem* BottomItem = ui->TablePredefinedIP->item(Row, i);

        QString TmpString = BottomItem->text();
        BottomItem->setText(TopItem->text());
        TopItem->setText(TmpString);
    }

    // Make the moved line the current one
    ui->TablePredefinedIP->selectRow(Row - 1);
}

void DlgConfiguration::moveDown()
{
    QList<QTableWidgetItem*> SelectedItems = ui->TablePredefinedIP->selectedItems();
    int                      Row           = ui->TablePredefinedIP->row(SelectedItems.at(0));
    int                      ColumnCount   = ui->TablePredefinedIP->columnCount();

    for (int i = 0; i < ColumnCount; i++) {
        QTableWidgetItem* TopItem    = ui->TablePredefinedIP->item(Row, i);
        QTableWidgetItem* BottomItem = ui->TablePredefinedIP->item(Row + 1, i);

        QString TmpString = BottomItem->text();
        BottomItem->setText(TopItem->text());
        TopItem->setText(TmpString);
    }

    // Make the moved line the current one
    ui->TablePredefinedIP->selectRow(Row + 1);
}

void DlgConfiguration::forgetConfiguration(Configuration* configuration)
{
    if (QMessageBox::question(
            this, QString("%1 - Forget interface").arg(APPLICATION_NAME), "Are you sure that you want to forget the settings of this interface?")
        == QMessageBox::Yes) {
        reject();
        Logger::instance()->addLogEntry(QString("Forgetting interface %1").arg(configuration->humanReadableName()));
        ConfigurationList::instance()->deleteConfiguration(configuration);
    }
}

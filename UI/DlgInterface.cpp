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

#include "DlgInterface.hpp"
#include "../Global.hpp"
#include "../Logger.hpp"
#include "../Network/InterfaceList.hpp"
#include "../Network/PredefinedIP.hpp"
#include "DlgPredefinedIP.hpp"
#include "ui_DlgInterface.h"
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

DlgInterface::DlgInterface(QNetworkInterface NetworkInterface)
    : ui(new Ui::DlgInterface)
    , Dialog(this)
{
    commonInitialization(InterfaceList::instance()->interface(NetworkInterface.hardwareAddress()));

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

DlgInterface::DlgInterface(Interface* interface)
    : ui(new Ui::DlgInterface)
    , Dialog(this)
{
    commonInitialization(interface);
    ui->EditName->setText(interface->humanReadableName());
    ui->EditHWaddress->setText(interface->hardwareAddress());
}

void DlgInterface::commonInitialization(Interface* interface)
{
    ui->setupUi(this);
    setWindowTitle(QString("%1 - Network Interface Configuration").arg(APPLICATION_NAME));
    ui->ButtonForgetInterface->setVisible(interface != nullptr);

    // Interface is null if no PredefinedIP has been defined for this device yet
    if (interface != nullptr) {
        QList<PredefinedIP*> IPlist = interface->predefinedIPlist();
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

    // Dialog
    connect(ui->ButtonOK, &QPushButton::clicked, this, [this]() { accept(); });
    connect(ui->ButtonCancel, &QPushButton::clicked, this, [this]() { reject(); });
    connect(ui->ButtonForgetInterface, &QPushButton::clicked, this, [this, interface]() {
        forgetInterface(interface);
    }); // Button not visible if interface is null

    // Table
    connect(ui->ButtoNewIP, &QPushButton::clicked, this, [this]() { newPredefinedIP(); });
    connect(ui->TablePredefinedIP, &QTableWidget::itemSelectionChanged, this, [this]() { tableSelectionChanged(); });
    connect(ui->TablePredefinedIP, &QTableWidget::doubleClicked, this, [this]() { editPredefinedIP(); });
    connect(ui->ButtonEditIP, &QPushButton::clicked, this, [this]() { editPredefinedIP(); });
    connect(ui->ButtonDeleteIP, &QPushButton::clicked, this, [this]() { deletePredefinedIP(); });
    connect(ui->ButtonUp, &QPushButton::clicked, this, [this]() { moveUp(); });
    connect(ui->ButtonDown, &QPushButton::clicked, this, [this]() { moveDown(); });
}

DlgInterface::~DlgInterface()
{
    delete ui;
}

void DlgInterface::tableSelectionChanged()
{
    QList<QTableWidgetItem*> SelectedItems = ui->TablePredefinedIP->selectedItems();

    // Defaut: no selection, buttons are disabled
    bool Enabled                           = false;
    bool AtTop                             = false;
    bool AtBottom                          = false;

    // Enable some of them if there is a valid selection
    if (SelectedItems.size() != 0) {
        Enabled  = true;
        int Row  = SelectedItems.at(0)->row();
        AtTop    = (Row == 0);
        AtBottom = (Row == ui->TablePredefinedIP->rowCount() - 1);
    }

    // Set buttons
    ui->ButtonEditIP->setEnabled(Enabled);
    ui->ButtonDeleteIP->setEnabled(Enabled);
    ui->ButtonUp->setEnabled(Enabled && !AtTop);
    ui->ButtonDown->setEnabled(Enabled && !AtBottom);
}

void DlgInterface::execDlgInterface(QNetworkInterface NetworkInterface)
{
    DlgInterface* Dlg = new DlgInterface(NetworkInterface);
    if (Dlg->exec() == QDialog::Accepted) {
        //
        // If the dialog is validated:
        // - if the interface does not exists yet and there is something to save
        //      - create the interface
        //      - write its new content
        //      - add the interface to the list
        // - if the interface already exists:
        //      - remove its content
        //      - write its new content
        //
        Interface* StoredInterface = InterfaceList::instance()->interface(NetworkInterface.hardwareAddress());

        if (StoredInterface == nullptr) {
            if (Dlg->ui->TablePredefinedIP->rowCount() != 0) {
                StoredInterface = new Interface(NetworkInterface.hardwareAddress(), NetworkInterface.humanReadableName());
                Dlg->writeContent(StoredInterface);
                InterfaceList::instance()->addInterface(StoredInterface);
            }
            // Nothing to do if we valitade an interface which does not exist yet and which has no content to store into
        }
        else {
            StoredInterface->clearContent();
            Dlg->writeContent(StoredInterface);
        }
    }
    delete Dlg;
}

void DlgInterface::execDlgInterface(Interface* interface)
{
    DlgInterface* Dlg = new DlgInterface(interface);
    // We are sure that the interface exists
    if (Dlg->exec() == QDialog::Accepted) {
        interface->clearContent();
        Dlg->writeContent(interface);
    }
    delete Dlg;
}

void DlgInterface::writeContent(Interface* interface)
{
    Logger::instance()->addLogEntry(QString("Setting Predefined IP for Interface %1").arg(interface->hardwareAddress()));

    for (int i = 0; i < ui->TablePredefinedIP->rowCount(); i++) {
        PredefinedIP* ip = new PredefinedIP(interface,
                                            ui->TablePredefinedIP->item(i, COLUMN_NAME)->text(),
                                            ui->TablePredefinedIP->item(i, COLUMN_IP_ADDRESS)->text(),
                                            ui->TablePredefinedIP->item(i, COLUMN_NETWORK_MASK)->text(),
                                            ui->TablePredefinedIP->item(i, COLUMN_GATEWAY)->text());
        interface->addPredefinedIP(ip);
        Logger::instance()->addLogEntry(QString("Name: %1. IP: %2. Mask: %3. Gateway: %4").arg(ip->name(), ip->ipAddress(), ip->networkMask(), ip->gateway()));
    }
}

void DlgInterface::newPredefinedIP()
{
    QString Name;
    QString IP;
    QString NetworkMask;
    QString Gateway;

    if (DlgPredefinedIP::newPredefinedIP(this, &Name, &IP, &NetworkMask, &Gateway) == QDialog::Accepted) {
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

void DlgInterface::editPredefinedIP()
{
    // The Edit button is enabled only if there are selected items, so SelectedItems is not empty
    QList<QTableWidgetItem*> SelectedItems = ui->TablePredefinedIP->selectedItems();
    int                      Row           = ui->TablePredefinedIP->row(SelectedItems.at(0));

    QString Name(ui->TablePredefinedIP->item(Row, COLUMN_NAME)->text());
    QString IP(ui->TablePredefinedIP->item(Row, COLUMN_IP_ADDRESS)->text());
    QString NetworkMask(ui->TablePredefinedIP->item(Row, COLUMN_NETWORK_MASK)->text());
    QString Gateway(ui->TablePredefinedIP->item(Row, COLUMN_GATEWAY)->text());

    if (DlgPredefinedIP::editPredefinedIP(this, &Name, &IP, &NetworkMask, &Gateway) == QDialog::Accepted) {
        ui->TablePredefinedIP->item(Row, COLUMN_NAME)->setText(Name);
        ui->TablePredefinedIP->item(Row, COLUMN_IP_ADDRESS)->setText(IP);
        ui->TablePredefinedIP->item(Row, COLUMN_NETWORK_MASK)->setText(NetworkMask);
        ui->TablePredefinedIP->item(Row, COLUMN_GATEWAY)->setText(Gateway);
    }
}

void DlgInterface::deletePredefinedIP()
{
    // The Delete button is enabled only if there are selected items, so SelectedItems is not empty
    QList<QTableWidgetItem*> SelectedItems = ui->TablePredefinedIP->selectedItems();
    int                      Row           = ui->TablePredefinedIP->row(SelectedItems.at(0));
    ui->TablePredefinedIP->removeRow(Row);
}

void DlgInterface::moveUp()
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
    // ui->TablePredefinedIP->selectRow(Row - 1);
}

void DlgInterface::moveDown()
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
    // ui->TablePredefinedIP->selectRow(Row + 1);
}

void DlgInterface::forgetInterface(Interface* interface)
{
    if (QMessageBox::question(this,
                              QString("%1 - Forget interface").arg(APPLICATION_NAME),
                              "Are you sure that you want to forget the settings of this interface?")
        == QMessageBox::Yes) {
        reject();
        InterfaceList::instance()->deleteInterface(interface);
        Logger::instance()->addLogEntry(QString("Forgetting interface %1").arg(interface->humanReadableName()));
    }
}

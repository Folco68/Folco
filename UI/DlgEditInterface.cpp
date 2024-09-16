#include "DlgEditInterface.hpp"
#include "../Network/InterfaceList.hpp"
#include "../Network/PredefinedIP.hpp"
#include "DlgEditPredefinedIP.hpp"
#include "ui_DlgEditInterface.h"
#include <QAbstractSocket>
#include <QHostAddress>
#include <QList>
#include <QNetworkAddressEntry>
#include <QPushButton>
#include <QTableWidgetItem>

// This dialog deals only with QString for editing a PredefinedIP, so that the Interface is not modified if the dialog is cancelled

DlgEditInterface::DlgEditInterface(QNetworkInterface NetworkInterface)
    : ui(new Ui::DlgEditInterface)
{
    ui->setupUi(this);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///                                                                                                                     ///
    ///                                                      Fill the header                                                ///
    ///                                                                                                                     ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
            break;
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///                                                                                                                     ///
    ///                                                      Fill the IP table                                              ///
    ///                                                                                                                     ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Interface* StoredInterface = InterfaceList::instance()->interface(NetworkInterface.hardwareAddress());

    if (StoredInterface != nullptr) {
        QList<PredefinedIP> IPlist = StoredInterface->predefinedIPlist();
        int                 Count  = IPlist.size();
        ui->TablePredefinedIP->setRowCount(Count);

        for (int i = 0; i < Count; i++) {
            PredefinedIP ip = IPlist.at(i);

            QTableWidgetItem* ItemName        = new QTableWidgetItem(ip.name());
            QTableWidgetItem* ItemIPaddress   = new QTableWidgetItem(ip.ipAddress());
            QTableWidgetItem* ItemNetworkMask = new QTableWidgetItem(ip.networkMask());
            QTableWidgetItem* ItemGateway     = new QTableWidgetItem(ip.gateway());

            ui->TablePredefinedIP->setItem(i, COLUMN_NAME, ItemName);
            ui->TablePredefinedIP->setItem(i, COLUMN_IP_ADDRESS, ItemIPaddress);
            ui->TablePredefinedIP->setItem(i, COLUMN_NETWORK_MASK, ItemNetworkMask);
            ui->TablePredefinedIP->setItem(i, COLUMN_GATEWAY, ItemGateway);
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///                                                                                                                     ///
    ///                                                         Connections                                                 ///
    ///                                                                                                                     ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Dialog
    connect(ui->ButtonOK, &QPushButton::clicked, this, [this]() { accept(); });
    connect(ui->ButtonCancel, &QPushButton::clicked, this, [this]() { reject(); });

    // Table
    connect(ui->ButtoNewIP, &QPushButton::clicked, this, [this]() { newPredefinedIP(); });
    connect(ui->TablePredefinedIP, &QTableWidget::itemSelectionChanged, this, [this]() { tableSelectionChanged(); });
    connect(ui->ButtonEditIP, &QPushButton::clicked, this, [this]() { editPredefinedIP(); });
    connect(ui->ButtonDeleteIP, &QPushButton::clicked, this, [this]() { deletePredefinedIP(); });
}

DlgEditInterface::~DlgEditInterface()
{
    delete ui;
}

void DlgEditInterface::tableSelectionChanged()
{
    QList<QTableWidgetItem*> SelectedItems = ui->TablePredefinedIP->selectedItems();
    bool                     Enabled       = SelectedItems.size() != 0;
    ui->ButtonEditIP->setEnabled(Enabled);
    ui->ButtonDeleteIP->setEnabled(Enabled);
    ui->ButtonUp->setEnabled(Enabled);
    ui->ButtonDown->setEnabled(Enabled);
}

void DlgEditInterface::execDlgEditInterface(QNetworkInterface NetworkInterface)
{
    DlgEditInterface* Dlg = new DlgEditInterface(NetworkInterface);
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
                StoredInterface = new Interface(NetworkInterface.hardwareAddress());
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

void DlgEditInterface::writeContent(Interface* interface)
{
    for (int i = 0; i < ui->TablePredefinedIP->rowCount(); i++) {
        PredefinedIP ip(ui->TablePredefinedIP->item(i, COLUMN_NAME)->text(),
                        ui->TablePredefinedIP->item(i, COLUMN_IP_ADDRESS)->text(),
                        ui->TablePredefinedIP->item(i, COLUMN_NETWORK_MASK)->text(),
                        ui->TablePredefinedIP->item(i, COLUMN_GATEWAY)->text());
        interface->addPredefinedIP(ip);
    }
}

void DlgEditInterface::newPredefinedIP()
{
    QString Name;
    QString IP;
    QString NetworkMask;
    QString Gateway;

    if (DlgEditPredefinedIP::newPredefinedIP(this, &Name, &IP, &NetworkMask, &Gateway) == QDialog::Accepted) {
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

void DlgEditInterface::editPredefinedIP()
{
    // The Edit button is enabled only if there are selected items, so SelectedItems is not empty
    QList<QTableWidgetItem*> SelectedItems = ui->TablePredefinedIP->selectedItems();
    int                      Row           = ui->TablePredefinedIP->row(SelectedItems.at(0));

    QString Name(ui->TablePredefinedIP->item(Row, COLUMN_NAME)->text());
    QString IP(ui->TablePredefinedIP->item(Row, COLUMN_IP_ADDRESS)->text());
    QString NetworkMask(ui->TablePredefinedIP->item(Row, COLUMN_NETWORK_MASK)->text());
    QString Gateway(ui->TablePredefinedIP->item(Row, COLUMN_GATEWAY)->text());

    if (DlgEditPredefinedIP::editPredefinedIP(this, &Name, &IP, &NetworkMask, &Gateway) == QDialog::Accepted) {
        ui->TablePredefinedIP->item(Row, COLUMN_NAME)->setText(Name);
        ui->TablePredefinedIP->item(Row, COLUMN_IP_ADDRESS)->setText(IP);
        ui->TablePredefinedIP->item(Row, COLUMN_NETWORK_MASK)->setText(NetworkMask);
        ui->TablePredefinedIP->item(Row, COLUMN_GATEWAY)->setText(Gateway);
    }
}

void DlgEditInterface::deletePredefinedIP()
{
    // The Delete button is enabled only if there are selected items, so SelectedItems is not empty
    QList<QTableWidgetItem*> SelectedItems = ui->TablePredefinedIP->selectedItems();
    int                      Row           = ui->TablePredefinedIP->row(SelectedItems.at(0));
    ui->TablePredefinedIP->removeRow(Row);
}

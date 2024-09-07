#include "DlgConfig.hpp"
#include "../Global.hpp"
#include "ui_DlgConfig.h"
#include <QAbstractItemView>
#include <QAbstractSocket>
#include <QCheckBox>
#include <QHostAddress>
#include <QList>
#include <QNetworkAddressEntry>
#include <QNetworkInterface>
#include <QString>
#include <QTableView>
#include <QTableWidget>
#include <QTableWidgetItem>

DlgConfig::DlgConfig(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::DlgConfig)
{
    // Setup window
    ui->setupUi(this);
    setWindowTitle(QString("%1 - Configuration").arg(APPLICATION_NAME));

    // Setup the interfaces table
    ui->TableInterfaces->setShowGrid(true);
    ui->TableInterfaces->setSortingEnabled(true);
    ui->TableInterfaces->setAlternatingRowColors(true);
    ui->TableInterfaces->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->TableInterfaces->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->TableInterfaces->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->TableInterfaces->verticalHeader()->setVisible(false);
    ui->TableInterfaces->horizontalHeader()->setStretchLastSection(true);

    // Populate the interface table
    refreshInterfaces();

    // Resize the columns only once and here (ie: not at the end of refreshInterfaces()), to avoid a setStretchSection() bug
    ui->TableInterfaces->resizeColumnsToContents();

    for (int i = 0; i < ui->TableInterfaces->columnCount(); i++) {
        ui->TableInterfaces->horizontalHeader()->resizeSection(i, ui->TableInterfaces->horizontalHeader()->sectionSize(i) + COLUMN_EXTRA_SIZE);
    }

    // Connections
    connect(ui->CheckShowOnlyIPv4, &QCheckBox::checkStateChanged, this, [this]() { refreshInterfaces(); });
    connect(ui->CheckShowOnlyAvailable, &QCheckBox::checkStateChanged, this, [this]() { refreshInterfaces(); });
    connect(ui->CheckKeepOnlyConfigured, &QCheckBox::checkStateChanged, this, [this]() { refreshInterfaces(); });
}

DlgConfig::~DlgConfig()
{
    delete ui;
}

int DlgConfig::execDlgConfig()
{
    DlgConfig* Dlg = new DlgConfig;
    Dlg->exec();
    delete Dlg;
    return 0;
}

void DlgConfig::refreshInterfaces()
{
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///                                                                                                                             ///
    ///                                                         Remove current data                                                 ///
    ///                                                                                                                             ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Retrieve table dimensions
    int RowCount    = ui->TableInterfaces->rowCount();
    int ColumnCount = ui->TableInterfaces->columnCount();

    // Delete the QTableWidgetItem of each cell
    for (int i = 0; i < RowCount; i++) {
        for (int j = 0; j < ColumnCount; j++) {
            delete ui->TableInterfaces->item(i, j);
        }
    }

    // All cells are empty, remove them
    ui->TableInterfaces->setRowCount(0);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///                                                                                                                             ///
    ///                                       List interfaces and display their data in the table                                   ///
    ///                                                                                                                             ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    QList<QNetworkInterface> Interfaces = QNetworkInterface::allInterfaces();

    for (int i = 0; i < Interfaces.size(); i++) {
        // Current interface
        QNetworkInterface Interface = Interfaces.at(i);

        // Filters
        if (ui->CheckShowOnlyIPv4->isChecked() && ((Interface.type() != QNetworkInterface::Ethernet) && (Interface.type() != QNetworkInterface::Wifi))) {
            continue;
        }

        if (ui->CheckShowOnlyAvailable->isChecked() && !(Interface.flags() & QNetworkInterface::IsUp)) {
            // Keep only configured
            continue;
        }

        // Interface properties
        // Default: IP and Network Mask are from the first entry of the list, regardeless of the protocol
        // The IPv4 IP and Network Mask will be retrieved right after and used if available
        QString      Name            = Interface.name();
        QString      LongName        = Interface.humanReadableName();
        QHostAddress IPaddress       = Interface.addressEntries().at(0).ip();
        QHostAddress NetworkMask     = Interface.addressEntries().at(0).netmask();
        QString      HardwareAddress = Interface.hardwareAddress();

        // Browse all the available addresses for this interface
        // Pick the IPv4 address if one is available, else stick to the default one
        QList<QNetworkAddressEntry> Addresses = Interface.addressEntries();
        for (int j = 0; j < Addresses.count(); j++) {
            if (Addresses.at(j).ip().protocol() == QAbstractSocket::IPv4Protocol) {
                IPaddress   = Addresses.at(j).ip();
                NetworkMask = Addresses.at(j).netmask();
            }
        }

        // Create graphical items
        QTableWidgetItem* ItemName            = new QTableWidgetItem(Name);
        QTableWidgetItem* ItemLongName        = new QTableWidgetItem(LongName);
        QTableWidgetItem* ItemIPaddress       = new QTableWidgetItem(IPaddress.toString());
        QTableWidgetItem* ItemNetworkMask     = new QTableWidgetItem(NetworkMask.toString());
        QTableWidgetItem* ItemHardwareAddress = new QTableWidgetItem(HardwareAddress);

        // Install graphical items
        int NewRowIndex = ui->TableInterfaces->rowCount();
        ui->TableInterfaces->setRowCount(NewRowIndex + 1);

        ui->TableInterfaces->setItem(NewRowIndex, COLUMN_NAME, ItemName);
        ui->TableInterfaces->setItem(NewRowIndex, COLUMN_LONG_NAME, ItemLongName);
        ui->TableInterfaces->setItem(NewRowIndex, COLUMN_IP_ADDRESS, ItemIPaddress);
        ui->TableInterfaces->setItem(NewRowIndex, COLUMN_NETWORK_MASK, ItemNetworkMask);
        ui->TableInterfaces->setItem(NewRowIndex, COLUMN_HARDWARE_ADDRESS, ItemHardwareAddress);
    }
}

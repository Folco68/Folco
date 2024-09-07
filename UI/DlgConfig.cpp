#include "DlgConfig.hpp"
#include "../Configuration/IFCfg.hpp"
#include "../Configuration/IFCfgList.hpp"
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

    // Finally, adjust the size to fit the content
    adjustSize();

    // Connections
    connect(ui->CheckShowOnlyIEthWiFi, &QCheckBox::checkStateChanged, this, [this]() { refreshInterfaces(); });
    connect(ui->CheckShowOnlyAvailable, &QCheckBox::checkStateChanged, this, [this]() { refreshInterfaces(); });
    connect(ui->CheckShowOnlyConfigured, &QCheckBox::checkStateChanged, this, [this]() { refreshInterfaces(); });
    connect(ui->TableInterfaces, &QTableWidget::itemSelectionChanged, this, [this]() { refreshEasySwitchTable(); });
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
    ///                                       List interfaces and display their data in the table                                   ///
    ///                                                                                                                             ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Clear the current content
    ui->TableInterfaces->clearContents();
    ui->TableInterfaces->setRowCount(0);

    // Grab all interfaces
    QList<QNetworkInterface> Interfaces = QNetworkInterface::allInterfaces();

    // Disable sorting to prevent sorting while we populate the line
    ui->TableInterfaces->setSortingEnabled(false);

    for (int i = 0; i < Interfaces.size(); i++) {
        // Current interface
        QNetworkInterface Interface = Interfaces.at(i);

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ///                                                                                                                     ///
        ///                                                         Filters                                                     ///
        ///                                                                                                                     ///
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        // Display only Ethernet and Wi-Fi interfaces
        if (ui->CheckShowOnlyIEthWiFi->isChecked() && ((Interface.type() != QNetworkInterface::Ethernet) && (Interface.type() != QNetworkInterface::Wifi))) {
            continue;
        }

        // Display only active interfaces
        if (ui->CheckShowOnlyAvailable->isChecked() && !(Interface.flags() & QNetworkInterface::IsUp)) {
            continue;
        }

        // Display only interfaces which have a configuration
        if (ui->CheckShowOnlyConfigured->isChecked() && !IFCfgList::instance()->hasConfiguration(Interface.hardwareAddress())) {
            continue;
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ///                                                                                                                     ///
        ///                                                         Display                                                     ///
        ///                                                                                                                     ///
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        // Interface properties
        // Default: IP and Network Mask are from the first entry of the list, regardeless of the protocol
        // The IPv4 IP and Network Mask will be retrieved right after and used if available
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
        QTableWidgetItem* ItemLongName        = new QTableWidgetItem(LongName);
        QTableWidgetItem* ItemIPaddress       = new QTableWidgetItem(IPaddress.toString());
        QTableWidgetItem* ItemNetworkMask     = new QTableWidgetItem(NetworkMask.toString());
        QTableWidgetItem* ItemHardwareAddress = new QTableWidgetItem(HardwareAddress);

        // Add one line to the table
        int NewRowIndex = ui->TableInterfaces->rowCount();
        ui->TableInterfaces->setRowCount(NewRowIndex + 1);

        // Insert items of the entry
        ui->TableInterfaces->setItem(NewRowIndex, COLUMN_IF_NAME, ItemLongName);
        ui->TableInterfaces->setItem(NewRowIndex, COLUMN_IF_IP_ADDRESS, ItemIPaddress);
        ui->TableInterfaces->setItem(NewRowIndex, COLUMN_IF_NETWORK_MASK, ItemNetworkMask);
        ui->TableInterfaces->setItem(NewRowIndex, COLUMN_IF_HARDWARE_ADDRESS, ItemHardwareAddress);
    }
    // Re-enable sorting
    ui->TableInterfaces->setSortingEnabled(true);
}

void DlgConfig::refreshEasySwitchTable()
{
    // Clear the current content
    ui->TableInterfaces->clearContents();
    ui->TableInterfaces->setRowCount(0);

    // Get the hw address of the current item
    int     CurrentRow      = ui->TableInterfaces->currentRow();
    QString HardwareAddress = ui->TableInterfaces->item(CurrentRow, COLUMN_IF_HARDWARE_ADDRESS)->text();

    // Retrieve the configuration of the current item if one exists
    IFCfg* Cfg = IFCfgList::instance()->ifCfg(HardwareAddress);
    if (Cfg != nullptr) {
        // Disable sorting to prevent sorting while we populate the line
        ui->TableEasySwitch->setSortingEnabled(false);

        QList<QVector<QString>> Configuration = Cfg->configuration();
        for (int i = 0; i < Configuration.size(); i++) {
            QVector<QString>  Entry           = Configuration[i];
            QTableWidgetItem* ItemIPaddress   = new QTableWidgetItem(Entry.at(0));
            QTableWidgetItem* ItemNetworkMask = new QTableWidgetItem(Entry.at(1));
            QTableWidgetItem* ItemGateway     = new QTableWidgetItem(Entry.at(2));

            // Add one line to the table
            int CurrentRow = ui->TableEasySwitch->rowCount();
            ui->TableEasySwitch->setRowCount(CurrentRow + 1);

            // Insert items of the entry
            ui->TableEasySwitch->setItem(i, COLUMN_CONFIG_IP_ADDRESS, ItemIPaddress);
            ui->TableEasySwitch->setItem(i, COLUMN_CONFIG_NETWORK_MASK, ItemNetworkMask);
            ui->TableEasySwitch->setItem(i, COLUMN_CONFIG_GATEWAY, ItemGateway);
        }

        // Disable sorting to prevent sorting while we populate the line
        ui->TableEasySwitch->setSortingEnabled(true);
    }
}

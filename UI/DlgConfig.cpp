#include "DlgConfig.hpp"
#include "../Configuration/IFConfig.hpp"
#include "../Configuration/IFConfigList.hpp"
#include "../Global.hpp"
#include "DlgEditConfig.hpp"
#include "ui_DlgConfig.h"
#include <QAbstractItemView>
#include <QAbstractSocket>
#include <QCheckBox>
#include <QHostAddress>
#include <QList>
#include <QNetworkAddressEntry>
#include <QNetworkInterface>
#include <QPushButton>
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

    // Setup the Interface table
    ui->TableInterface->setShowGrid(true);
    ui->TableInterface->setSortingEnabled(true);
    ui->TableInterface->setAlternatingRowColors(true);
    ui->TableInterface->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->TableInterface->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->TableInterface->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->TableInterface->verticalHeader()->setVisible(false);
    ui->TableInterface->horizontalHeader()->setStretchLastSection(true);

    // Populate the Interface table
    refreshInterface();

    // Resize the columns only once and here (ie: not at the end of refreshInterfaces()), to avoid a setStretchSection() bug
    ui->TableInterface->resizeColumnsToContents();

    for (int i = 0; i < ui->TableInterface->columnCount(); i++) {
        ui->TableInterface->horizontalHeader()->resizeSection(i, ui->TableInterface->horizontalHeader()->sectionSize(i) + COLUMN_EXTRA_SIZE);
    }

    // Finally, adjust the size to fit the content
    adjustSize();

    // Connections
    connect(ui->CheckShowOnlyIEthWiFi, &QCheckBox::checkStateChanged, this, [this]() { refreshInterface(); });
    connect(ui->CheckShowOnlyAvailable, &QCheckBox::checkStateChanged, this, [this]() { refreshInterface(); });
    connect(ui->CheckShowOnlyConfigured, &QCheckBox::checkStateChanged, this, [this]() { refreshInterface(); });
    connect(ui->TableInterface, &QTableWidget::itemSelectionChanged, this, [this]() { interfaceSelectionChanged(); });
    connect(ui->ButtonAdd, &QPushButton::clicked, this, [this]() { addConfiguration(); });
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

void DlgConfig::refreshInterface()
{
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///                                                                                                                             ///
    ///                                       List interfaces and display their data in the table                                   ///
    ///                                                                                                                             ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Clear the current content
    ui->TableInterface->clearContents();
    ui->TableInterface->setRowCount(0);

    // Grab all interfaces
    QList<QNetworkInterface> Interfaces = QNetworkInterface::allInterfaces();

    // Disable sorting to prevent sorting while we populate the line
    ui->TableInterface->setSortingEnabled(false);

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
        if (ui->CheckShowOnlyConfigured->isChecked() && !IFConfigList::instance()->hasConfiguration(Interface.hardwareAddress())) {
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
        int NewRowIndex = ui->TableInterface->rowCount();
        ui->TableInterface->setRowCount(NewRowIndex + 1);

        // Insert items of the entry
        ui->TableInterface->setItem(NewRowIndex, COLUMN_IF_NAME, ItemLongName);
        ui->TableInterface->setItem(NewRowIndex, COLUMN_IF_IP_ADDRESS, ItemIPaddress);
        ui->TableInterface->setItem(NewRowIndex, COLUMN_IF_NETWORK_MASK, ItemNetworkMask);
        ui->TableInterface->setItem(NewRowIndex, COLUMN_IF_HARDWARE_ADDRESS, ItemHardwareAddress);
    }
    // Re-enable sorting
    ui->TableInterface->setSortingEnabled(true);
}

void DlgConfig::interfaceSelectionChanged()
{
    // Clear the current content of the Configuration Table,
    // because it will be adapted to the selected interface
    ui->TablePredefined->clearContents();
    ui->TablePredefined->setRowCount(0);

    // Don't perform anything if there is no interface selected
    if (!ui->TableInterface->selectedItems().empty()) {

        // Get the hw address of the current item
        int     CurrentRow      = ui->TableInterface->currentRow();
        QString HardwareAddress = ui->TableInterface->item(CurrentRow, COLUMN_IF_HARDWARE_ADDRESS)->text();

        // Retrieve the configuration of the current item if one exists
        IFConfig* Interface = IFConfigList::instance()->ifConfig(HardwareAddress);
        if (Interface != nullptr) {
            // Disable sorting while we populate the line
            ui->TablePredefined->setSortingEnabled(false);
            QList<Configuration> ConfigList = Interface->configurations();

            for (int i = 0; i < ConfigList.size(); i++) {
                Configuration     Config          = ConfigList.at(i);
                QTableWidgetItem* ItemIPaddress   = new QTableWidgetItem(Config.ipAddress());
                QTableWidgetItem* ItemNetworkMask = new QTableWidgetItem(Config.networkMask());
                QTableWidgetItem* ItemGateway     = new QTableWidgetItem(Config.gateway());

                // Add one line to the table
                int CurrentRow = ui->TablePredefined->rowCount();
                ui->TablePredefined->setRowCount(CurrentRow + 1);

                // Insert items of the entry
                ui->TablePredefined->setItem(i, COLUMN_CONFIG_IP_ADDRESS, ItemIPaddress);
                ui->TablePredefined->setItem(i, COLUMN_CONFIG_NETWORK_MASK, ItemNetworkMask);
                ui->TablePredefined->setItem(i, COLUMN_CONFIG_GATEWAY, ItemGateway);
            }

            // Disable sorting to prevent sorting while we populate the line
            ui->TablePredefined->setSortingEnabled(true);
        }

        // If an interface is selected, we can always add a new configuration
        ui->ButtonAdd->setEnabled(true);
    }
    // Nothing is selected in the Interface table, so we can't add any configuration
    else {
        ui->ButtonAdd->setDisabled(true);
    }
}

void DlgConfig::addConfiguration()
{
    Configuration Config = DlgEditConfig::dlgNewConfig(this);
    // Add a new configuration to the current inteface if the dialog was validated
    if (Config.isValid()) {
        int     CurrentRow      = ui->TableInterface->currentRow();
        QString HardwareAddress = ui->TableInterface->item(CurrentRow, COLUMN_IF_HARDWARE_ADDRESS)->text();
        IFConfigList::instance()->addConfiguration(HardwareAddress, Config);
    }
}

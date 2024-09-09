#include "TrayIcon.hpp"
#include "DlgSettings.hpp"
#include <QCoreApplication>
#include <QCursor>
#include <QIcon>

TrayIcon::TrayIcon()
    : QSystemTrayIcon {QIcon(":/Icons/IconBase.png")}
{
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///                                                                                                                             ///
    ///                                          Create the base of the tray icon context menu                                      ///
    ///                                                                                                                             ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    QMenu* ContextMenu = new QMenu;

    // Create actions. Give the parentality to ContextMenu, avoiding to destroy items by hand on exit
    QAction* ActionSettings = new QAction("Settings...", ContextMenu);
    QAction* ActionAbout    = new QAction("About...", ContextMenu);
    QAction* ActionExit     = new QAction("Exit", ContextMenu);

    // Populate the context menu
    ContextMenu->addSeparator();
    ContextMenu->addAction(ActionSettings);
    ContextMenu->addAction(ActionAbout);
    ContextMenu->addSeparator();
    ContextMenu->addAction(ActionExit);

    // Add the context menu to the tray icon
    setContextMenu(ContextMenu);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///                                                                                                                             ///
    ///                                                     Set base connections                                                    ///
    ///                                                                                                                             ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Show the context menu regardless of the trigger (default: only the right click displays the menu)
    connect(this, &QSystemTrayIcon::activated, this, [ContextMenu]() { ContextMenu->popup(QCursor::pos()); });

    // Configuration dialog
    connect(ActionSettings, &QAction::triggered, []() { DlgSettings::execDlgSettings(); });

    // Exit the application
    connect(ActionExit, &QAction::triggered, []() { QCoreApplication::exit(0); });
}

/*
void DlgSettings::refreshInterface()
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
*/

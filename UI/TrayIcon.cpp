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

#include "TrayIcon.hpp"
#include <QAction>
#include <QCoreApplication>
#include <QCursor>
#include <QIcon>
#include <QList>
#include <QMenu>
#include <QNetworkAddressEntry>
#include <QNetworkInterface>
#include <QProcess>
#include <QString>
#include "../Global.hpp"
#include "../Logger.hpp"
#include "../Network/InterfaceList.hpp"
#include "../Network/SystemInterfaces.hpp"
#include "../Settings.hpp"
#include "Dialog.hpp"
#include "DlgHelp.hpp"
#include "DlgInterface.hpp"
#include "DlgLog.hpp"
#include "DlgSettings.hpp"

TrayIcon::TrayIcon()
    : QSystemTrayIcon {QIcon(":/Icons/IconBase.png")}
    , ContextMenu(nullptr)
{
    Logger::instance()->addLogEntry("Folco started...");
    setToolTip(APPLICATION_NAME " - " APPLICATION_DESCRIPTION);

    // The menu is created dynamically every time it is triggerred, to refresh the interface list
    connect(this,
            &QSystemTrayIcon::activated, // Show the context menu regardless of the trigger (default: only the right click displays the menu)
            this,
            [this]() {
                if (!Dialog::display()) {            // If a window is already displayed, bring it to the foreground instead of showing the menu
                    showContextMenu(QCursor::pos()); // Read the cursor position now, to prevent a dialog box from moving the menu before it pops up
                };
            });
}

TrayIcon::~TrayIcon()
{
    if (this->ContextMenu != nullptr) {
        delete this->ContextMenu;
        this->ContextMenu = nullptr;
    }

    InterfaceList::release();
    Settings::release();
    Logger::release();
}

void TrayIcon::showContextMenu(QPoint position)
{
    // Delete the previous menu if one was already created
    if (this->ContextMenu != nullptr) {
        delete this->ContextMenu;
        this->ContextMenu = nullptr;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///                                                                                                                             ///
    ///                           Generate the list of interfaces according to the user-defined settings                            ///
    ///                                                                                                                             ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    QList<QNetworkInterface> FilteredInterfaces = SystemInterfaces::filteredInterfaces();

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///                                                                                                                     ///
    ///          Create the dynamic part of the menu, which contains the interfaces with predefined IP address              ///
    ///                                                                                                                     ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    this->ContextMenu = new QMenu;

    // Section title. Can't use QMenu::addSection because Windows 10/11 don't display it
    QAction* Title = new QAction("*** Host interfaces ***");
    Title->setDisabled(true);
    this->ContextMenu->addAction(Title);

    for (int i = 0; i < FilteredInterfaces.size(); i++) {
        // Create the Interface item in the main menu
        QNetworkInterface NetworkInterface = FilteredInterfaces.at(i);                          // Current Network Interface
        QAction* ActionNetworkInterface    = new QAction(NetworkInterface.humanReadableName()); // Action (item) of this Network Interface

        // Initialize some other vars
        QString HardwareAddress = NetworkInterface.hardwareAddress();   // HW address of this Network Interface
        QString Name            = NetworkInterface.humanReadableName(); // Name, used to identify the interface in the netsh commands

        // There is no stored Interface if there is not at least one predefined IP <===== TODO: this should be false now
        // So, set the IP count to 0 by default, and update it only if there is really predefined IP
        Interface* StoredInterface = InterfaceList::instance()->interface(HardwareAddress); // Stored Interface with predefined IP
        int        IPcount         = 0;                                                     // Count of predefined IP for this Stored Interface
        if (StoredInterface != nullptr) {
            IPcount = StoredInterface->predefinedIPcount();

            // Add the custom name if one exists
            if (!StoredInterface->customName().isEmpty()) {
                ActionNetworkInterface->setText(QString("%1 (%2)").arg(ActionNetworkInterface->text(), StoredInterface->customName()));
            }
        }

        // Create the sub-menu containing the predefined IP and the "Edit predefined IP" item
        QMenu* Submenu = new QMenu;

        // Create the dynamic part of the menu with the predefined IP
        if (IPcount != 0) {
            QList<PredefinedIP*> PredefinedIPList = StoredInterface->predefinedIPlist();
            for (int j = 0; j < IPcount; j++) {
                PredefinedIP* IP       = PredefinedIPList.at(j);
                QAction*      ActionIP = new QAction(QString("%1: %2").arg(IP->name(), IP->ipAddress()));
                Submenu->addAction(ActionIP);
                connect(ActionIP, &QAction::triggered, this, [this, Name, IP]() { configureInterfacePredefinedIP(Name, IP); });
            }
        }

        // Add the "Use DHCP" item
        QAction* ActionUseDHCP = new QAction("Use DHCP");
        Submenu->addSeparator();
        Submenu->addAction(ActionUseDHCP);
        // TODO: get ipv4 address if one exists to delete it (if needed: call configureInterfaceDHCP() with a default argument)x
        connect(ActionUseDHCP, &QAction::triggered, this, [this, Name]() { configureInterfaceDHCP(Name); });

        // Add the "Edit predefined IP" item
        QAction* ActionEditPredefinedIP = new QAction("Edit predefined IP");
        Submenu->addSeparator();
        Submenu->addAction(ActionEditPredefinedIP);
        connect(ActionEditPredefinedIP, &QAction::triggered, this, [NetworkInterface]() { DlgInterface::execDlgInterface(NetworkInterface); });

        // Fill the main context menu
        this->ContextMenu->addAction(ActionNetworkInterface);
        ActionNetworkInterface->setMenu(Submenu);
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///                                                                                                                     ///
    ///          Create the second dynamic part of the menu, which contains the known but unconnected interfaces            ///
    ///                                                                                                                     ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    QList<QNetworkInterface> AllInterfaces = QNetworkInterface::allInterfaces();
    QList<Interface*> InterfaceList(InterfaceList::instance()->interfaceList());
    QList<QString>    HardwareAddresses;
    bool              DisconnectedInterfaces = false;

    // Generate the list of filtered hw addresses. Does not take filters in account
    // to avoid displaying PredefinedIP of masked hardaware interface
    for (int i = 0; i < AllInterfaces.size(); i++) {
        HardwareAddresses.append(AllInterfaces.at(i).hardwareAddress());
    }

    // Check if at least one Interface exists, but its hw device is disconnected
    for (int i = 0; i < InterfaceList.size(); i++) {
        if (!HardwareAddresses.contains(InterfaceList.at(i)->hardwareAddress())) {
            DisconnectedInterfaces = true;
            break;
        }
    }

    if (DisconnectedInterfaces) {
        // Generate the menu header
        QAction* Title = new QAction("*** Disconnected interfaces ***");
        Title->setDisabled(true);
        this->ContextMenu->addSeparator();
        this->ContextMenu->addAction(Title);

        for (int i = 0; i < InterfaceList.size(); i++) {
            if (!HardwareAddresses.contains(InterfaceList.at(i)->hardwareAddress())) {
                QAction* ActionNetworkInterface = new QAction(InterfaceList.at(i)->humanReadableName());
                this->ContextMenu->addAction(ActionNetworkInterface);
                QMenu*   Submenu                = new QMenu;
                QAction* ActionEditPredefinedIP = new QAction("Edit predefined IP");
                Submenu->addAction(ActionEditPredefinedIP);
                ActionNetworkInterface->setMenu(Submenu);
                connect(ActionEditPredefinedIP, &QAction::triggered, this, [InterfaceList, i]() { DlgInterface::execDlgInterface(InterfaceList.at(i)); });

                // Add custom name if one is set
                if (!InterfaceList.at(i)->customName().isEmpty()) {
                    ActionNetworkInterface->setText(QString("%1 (%2)").arg(ActionNetworkInterface->text(), InterfaceList.at(i)->customName()));
                }
            }
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///                                                                                                                     ///
    ///                                                 Fill the end of the menu                                            ///
    ///                                                                                                                     ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Create the actions
    QAction* ActionTools = new QAction("Tools");
    QAction* ActionAbout = new QAction("About");
    QAction* ActionExit  = new QAction("Exit");

    // Fill the menu
    this->ContextMenu->addSeparator();
    this->ContextMenu->addAction(ActionTools);
    this->ContextMenu->addAction(ActionAbout);
    this->ContextMenu->addSeparator();
    this->ContextMenu->addAction(ActionExit);

    // Tools menu
    QAction* ActionSettings = new QAction("Settings");
    QAction* ActionLog      = new QAction("Log");
    QAction* ActionMerge;
    if (DisconnectedInterfaces) {
        ActionMerge = new QAction("Merge connections");
    }

    QMenu* ToolMenu = new QMenu;
    ToolMenu->addAction(ActionSettings);
    ToolMenu->addAction(ActionLog);
    if (DisconnectedInterfaces) {
        ToolMenu->addAction(ActionMerge);
    }
    ActionTools->setMenu(ToolMenu);

    // Connect the actions
    connect(ActionSettings, &QAction::triggered, []() { DlgSettings::execDlgSettings(); });
    //    connect(ActionMerge, &QAction::triggered, []() { DlgMerge::execDlgMerge(); });
    connect(ActionLog, &QAction::triggered, []() { DlgLog::showDlgLog(); });
    connect(ActionAbout, &QAction::triggered, []() { DlgHelp::showDlgHelp(); });
    connect(ActionExit, &QAction::triggered, []() { QCoreApplication::exit(0); });

    // Add the context menu to the tray icon
    setContextMenu(this->ContextMenu);

    // Finally, show the context menu at cursor position
    this->ContextMenu->popup(position);
}

void TrayIcon::configureInterfacePredefinedIP(QString name, PredefinedIP* ip)
{
    // Set the IPv4 address with its mask and gateway
    // $ netsh interface ipv4 set address "[interface name]" static [IP] [netmask]
    QList<QString> ArgAddress;
    ArgAddress << QString("/c netsh interface ipv4 set address \"%1\" static %2 %3 %4").arg(name, ip->ipAddress(), ip->networkMask(), ip->gateway());
    QProcess::execute("cmd.exe", ArgAddress);
    Logger::instance()->addLogEntry(tr("Setting predefined IP: %1").arg(ArgAddress.at(0)));
}

// QProcess::execute() is synchronous, so no need to listen to QProcess::finished to schedule the three calls
void TrayIcon::configureInterfaceDHCP(QString name)
{
    Logger::instance()->addLogEntry(tr("Configuring %1 for DHCP").arg(name));

    // 1. Delete the IPv4 addresses of this interface it some are set
    // $ netsh interface ipv4 delete address "[interface name]" addr=[IP]
    QNetworkInterface           NetworkInteface = QNetworkInterface::interfaceFromName(name);
    QList<QNetworkAddressEntry> AddressEntries  = NetworkInteface.addressEntries();
    for (int i = 0; i < AddressEntries.size(); i++) {
        QHostAddress HostAddress = AddressEntries.at(i).ip();
        if (HostAddress.protocol() == QAbstractSocket::IPv4Protocol) {
            QList<QString> ArgDelete;
            ArgDelete << QString("/c netsh interface ipv4 delete address \"%1\" addr=%2").arg(name, HostAddress.toString());
            QProcess::execute("cmd.exe", ArgDelete);
            // Don't break, because an interface can have multiple IPv4 addresses

            Logger::instance()->addLogEntry(tr("Deleting IP: %1").arg(ArgDelete.at(0)));
        }
    }

    // 2. Enable DHCP for the address/network mask
    // $ netsh interface ipv4 set address "[interface name]" dhcp
    QList<QString> ArgAddress;
    ArgAddress << QString("/c netsh interface ipv4 set address \"%1\" dhcp").arg(name);
    QProcess::execute("cmd.exe", ArgAddress);
    Logger::instance()->addLogEntry(tr("Enabling DHCP for the Eth address: %1").arg(ArgAddress.at(0)));

    // 3. Enable DHCP for the DNS servers
    // $ netsh interface ipv4 set dns "[interface name]" dhcp
    QList<QString> ArgDNS;
    ArgDNS << QString("/c netsh interface ipv4 set dns \"%1\" dhcp").arg(name);
    QProcess::execute("cmd.exe", ArgDNS);
    Logger::instance()->addLogEntry(tr("Enabling DHCP for the DNS servers: %1").arg(ArgAddress.at(0)));
}

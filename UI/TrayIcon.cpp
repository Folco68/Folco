#include "TrayIcon.hpp"
#include "../Network/InterfaceList.hpp"
#include "../Settings.hpp"
#include "DlgEditInterface.hpp"
#include "DlgSettings.hpp"
#include <QAction>
#include <QCoreApplication>
#include <QCursor>
#include <QIcon>
#include <QList>
#include <QMenu>
#include <QNetworkInterface>
#include <QString>

TrayIcon::TrayIcon()
    : QSystemTrayIcon {QIcon(":/Icons/IconBase.png")}
    , ContextMenu(nullptr)
{
    // Show the context menu regardless of the trigger (default: only the right click displays the menu)
    // The menu is created dynamically every time it is triggerred, to refresh the interface list
    connect(this, &QSystemTrayIcon::activated, this, [this]() { showContextMenu(); });
}

TrayIcon::~TrayIcon()
{
    if (this->ContextMenu != nullptr) {
        delete this->ContextMenu;
        this->ContextMenu = nullptr;
    }
}

void TrayIcon::showContextMenu()
{
    // Delete the previous menu if one was already created
    if (this->ContextMenu != nullptr) {
        delete this->ContextMenu;
        this->ContextMenu = nullptr;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///                                                                                                                             ///
    ///                                   Generate the list of interfaces according to the settings                                 ///
    ///                                                                                                                             ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    QList<QNetworkInterface> AllInterfaces = QNetworkInterface::allInterfaces();
    QList<QNetworkInterface> FilteredNetworkInterfaces;

    for (int i = 0; i < AllInterfaces.size(); i++) {

        // Current interface
        QNetworkInterface NetworkInterface = AllInterfaces.at(i);

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ///                                                                                                                     ///
        ///                               Loop if the current interface matches the filter criteria                             ///
        ///                                                                                                                     ///
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        if (Settings::instance()->showOnlyEthernetWifi() && NetworkInterface.type() != QNetworkInterface::Ethernet && NetworkInterface.type() != QNetworkInterface::Wifi) {
            continue;
        }

        if (Settings::instance()->showOnlyUp() && !(NetworkInterface.flags() & QNetworkInterface::IsUp)) {
            continue;
        }

        if (Settings::instance()->showOnlyPredefined() && !InterfaceList::instance()->hasConfiguration(NetworkInterface.hardwareAddress())) {
            continue;
        }

        // This interface matches all criteria and will be part of the menu
        FilteredNetworkInterfaces << NetworkInterface;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///                                                                                                                     ///
    ///          Create the dynamic part of the menu, which contains the interfaces with predefined IP address              ///
    ///                                                                                                                     ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    this->ContextMenu = new QMenu;

    for (int i = 0; i < FilteredNetworkInterfaces.size(); i++) {
        // Create the Interface item in the main menu
        QNetworkInterface NetworkInterface       = FilteredNetworkInterfaces.at(i);                   // Current Network Interface
        QAction*          ActionNetworkInterface = new QAction(NetworkInterface.humanReadableName()); // Name of this Network Interface
        QString           HardwareAddress        = NetworkInterface.hardwareAddress();                // HW address of this Network Interface

        // There is no stored Interface if there is not at least one predefined IP
        // So, set the IP count to 0 by default, and update it only if there is really predefined IP
        Interface* Interface = InterfaceList::instance()->interface(HardwareAddress); // Stored Interface with predefined IP
        int        IPcount   = 0;                                                     // Count of predefined IP for this Stored Interface
        if (Interface != nullptr) {
            IPcount = Interface->predefinedIPcount();
        }

        // Create the sub-menu containing the predefined IP and the "Edit predefined IP" item
        QMenu* Submenu = new QMenu;

        if (IPcount != 0) {
            QList<PredefinedIP> PredefinedIPList = Interface->predefinedIPlist();
            for (int j = 0; j < IPcount; j++) {
                PredefinedIP IP = PredefinedIPList.at(i);
                QString      IPstring(IP.ipAddress());
                // TODO: complete with netmask?
                QAction* ActionIP = new QAction(IPstring);

                Submenu->addAction(ActionIP);
                // TODO                connect(ActionIP, &QAction::triggered, this, setInterfaceIPwithNetSH());
            }
        }

        // Add the "Edit predefined IP" item
        QAction* ActionEditPredefinedIP = new QAction("Edit predefined IP");
        Submenu->addSeparator();
        Submenu->addAction(ActionEditPredefinedIP);
        connect(ActionEditPredefinedIP, &QAction::triggered, [HardwareAddress]() { DlgEditInterface::execDlgEditInterface(HardwareAddress); });

        // Fil the main context menu
        this->ContextMenu->addAction(ActionNetworkInterface);
        ActionNetworkInterface->setMenu(Submenu);
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///                                                                                                                     ///
    ///                                                 Fill the end of the menu                                            ///
    ///                                                                                                                     ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Create the actions
    QAction* ActionSettings = new QAction("Settings");
    QAction* ActionAbout    = new QAction("About");
    QAction* ActionExit     = new QAction("Exit");

    // Fill the menu
    this->ContextMenu->addSeparator();
    this->ContextMenu->addAction(ActionSettings);
    this->ContextMenu->addAction(ActionAbout);
    this->ContextMenu->addSeparator();
    this->ContextMenu->addAction(ActionExit);

    // Connect the actions
    connect(ActionSettings, &QAction::triggered, []() { DlgSettings::execDlgSettings(); });
    //    connect(ActionAbout, &QAction::triggered, []() { DlgAbout::execDlgAbout(); });
    connect(ActionExit, &QAction::triggered, []() {
        Settings::release();
        QCoreApplication::exit(0);
    });

    // Add the context menu to the tray icon
    setContextMenu(this->ContextMenu);

    // Finally, show the context menu at cursor position
    this->ContextMenu->popup(QCursor::pos());
}

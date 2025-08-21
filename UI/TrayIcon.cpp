#include "TrayIcon.hpp"
#include "../Global.hpp"
#include "../Logger.hpp"
#include "../Network/Configuration.hpp"
#include "../Network/ConfigurationList.hpp"
#include "../Settings.hpp"
#include "Dialog.hpp"
#include "DlgConfiguration.hpp"
#include <QList>
#include <QNetworkInterface>
#include <QPair>
#include <QProcess>

TrayIcon::TrayIcon()
    : QSystemTrayIcon{QIcon(":/Icons/IconBase.png")}
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

    ConfigurationList::release();
    Settings::release();
    Logger::release();
}

void TrayIcon::showContextMenu(QPoint position)
{
    // Delete previous menu if one exists
    if (this->ContextMenu != nullptr) {
        delete this->ContextMenu;
    }

    this->ContextMenu = new QMenu;

    // There are three kinds of items:
    // - Network Interfaces without Configuration
    // - Network Interfaces with Configuration
    // - Configuration existing without a Network Interface (unplugged, disabled, ...)
    //
    // How the menu is generated:
    // - Start from 2 lists: Network Interfaces and Configurations
    // - Create a list of pairs <Network Interface / Configuration*>. Configuration may be nullptr. Remove Configuration from their list when they match a Network Interface
    // - Filter the list of pairs with user settings
    // - Populate the menu with the list of pairs. The submenu will be populated according to the existence of a configuration
    // - Finally, add the remaining items of the Configuration list in a "Disconnected interfaces"

    QList<QPair<QNetworkInterface, Configuration*>> GlobalList;
    QList<QNetworkInterface>                        NetworkInterfaceList(QNetworkInterface::allInterfaces());
    QList<Configuration*>                           ConfigurationList(ConfigurationList::instance()->configurationList());

    for (int i = 0; i < NetworkInterfaceList.size(); i++) {
        QNetworkInterface NetworkInterface = NetworkInterfaceList.at(i);
        Configuration*    Config           = nullptr;
        for (int j = 0; j < ConfigurationList.size(); j++) {
            if (NetworkInterface.hardwareAddress() == ConfigurationList.at(j)->hardwareAddress()) {
                Config = ConfigurationList.takeAt(j--);
                break;
            }
        }

        // Apply the user settings
        if ((Settings::instance()->showOnlyEthernetWifi())
            && (!(NetworkInterface.type() != QNetworkInterface::Ethernet) || !(NetworkInterface.type() != QNetworkInterface::Wifi))) {
            continue;
        }

        if (Settings::instance()->showOnlyPredefined() && (Config != nullptr) && (!Config->hasPredefinedIP())) {
            continue;
        }

        if (Settings::instance()->showOnlyUp() && !(NetworkInterface.flags() & QNetworkInterface::IsUp)) {
            continue;
        }

        // This entry is not filtered, add it to the global list
        GlobalList.append(QPair<QNetworkInterface, Configuration*>(NetworkInterface, Config));
    }

    // Create a section "Host interfaces" if at least one Network Interface must be displayed
    if (!GlobalList.isEmpty()) {
        // Section title. Don't use regular addSection because rendering is not guaranted
        QAction* ActionHostInterfaces = new QAction("Host interfaces", this->ContextMenu);
        ActionHostInterfaces->setDisabled(true);
        this->ContextMenu->addAction(ActionHostInterfaces);

        // Network Interfaces
        for (int i = 0; i < GlobalList.size(); i++) {
            QNetworkInterface NetworkInterface = GlobalList.at(i).first;
            Configuration*    Configuration    = GlobalList.at(i).second;

            // Network Interface name
            QString TitleNetworkInterface(GlobalList.at(i).first.humanReadableName());
            if (GlobalList.at(i).second != nullptr) {
                QString CustomName = GlobalList.at(i).second->customName();
                TitleNetworkInterface.append(QString(" - %1").arg(CustomName));
            }

            // Network Interface action and submenu
            QAction* ActionNetworkInterface = new QAction(TitleNetworkInterface, this->ContextMenu);
            this->ContextMenu->addAction(ActionNetworkInterface);
            QMenu* Submenu = new QMenu(this->ContextMenu);
            ActionNetworkInterface->setMenu(Submenu);

            // Add PDI
            if (Configuration != nullptr) {
                QList<PredefinedIP*> PredefinedIPlist = Configuration->predefinedIPlist();
                for (int j = 0; j < PredefinedIPlist.size(); j++) {
                    PredefinedIP* PredefinedIP = PredefinedIPlist.at(j);

                    // PDI name
                    QString NamePDI(PredefinedIP->ipAddress());
                    if (!PredefinedIP->name().isEmpty()) {
                        NamePDI.prepend(QString("%1 - ").arg(PredefinedIP->name()));
                    }

                    // PDI action
                    QAction* ActionPDI = new QAction(NamePDI, this->ContextMenu);
                    Submenu->addAction(ActionPDI);
                    connect(ActionPDI, &QAction::triggered, this, [this, Configuration, PredefinedIP]() {
                        configureInterfacePredefinedIP(Configuration->humanReadableName(), PredefinedIP);
                    });
                }

                // Separate PDI from DHCP
                if (!PredefinedIPlist.isEmpty()) {
                    Submenu->addSeparator();
                }
            }

            // Add DHCP configuration
            QAction* ActionDHCP = new QAction("DHCP", this->ContextMenu);
            Submenu->addAction(ActionDHCP);
            connect(ActionDHCP, &QAction::triggered, this, [this, Configuration]() { configureInterfaceDHCP(Configuration->humanReadableName()); });

            // Add Network Interface edition
            Submenu->addSeparator();
            QAction* ActionEditInterface = new QAction("Edit pre-defined IP", this->ContextMenu);
            Submenu->addAction(ActionEditInterface);
            connect(ActionEditInterface, &QAction::triggered, this, [NetworkInterface]() { DlgConfiguration::execDlgConfiguration(NetworkInterface); });
        }
    }

    // Create a section "Disconnected interfaces" if at least one Configuration is orphean
    if (!ConfigurationList.isEmpty()) {
        // Section title
        this->ContextMenu->addSeparator();
        QAction* DisconnectedInterfaces = new QAction("Disconnected interfaces", this->ContextMenu);
        DisconnectedInterfaces->setDisabled(true);
        this->ContextMenu->addAction(DisconnectedInterfaces);

        // Disconnected interfaces (ie. pure configurations)
        for (int i = 0; i < ConfigurationList.size(); i++) {
            Configuration* Configuration = ConfigurationList.at(i);

            // Configuration name
            QString TitleConfiguration(Configuration->humanReadableName());
            if (!Configuration->customName().isEmpty()) {
                TitleConfiguration.append(QString(" - %1").arg(Configuration->customName()));
            }

            // Configuration action
            QAction* ActionConfiguration = new QAction(TitleConfiguration, this->ContextMenu);
            this->ContextMenu->addAction(ActionConfiguration);

            // Submenu
            QMenu* Submenu = new QMenu(this->ContextMenu);
            ActionConfiguration->setMenu(Submenu);

            // Edit Configuration action
            QAction* ActionEditConfiguration = new QAction("Edit configuration", this->ContextMenu);
            Submenu->addAction(ActionEditConfiguration);
            connect(ActionEditConfiguration, &QAction::triggered, this, [Configuration]() { DlgConfiguration::execDlgConfiguration(Configuration); });
        }
    }

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

#include "DlgEditInterface.hpp"
#include "ui_DlgEditInterface.h"

DlgEditInterface::DlgEditInterface(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::DlgEditInterface)
{
    ui->setupUi(this);
}

DlgEditInterface::~DlgEditInterface()
{
    delete ui;
}

void DlgEditInterface::execDlgEditInterface(QString hwaddress)
{
}

/*    
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
*/

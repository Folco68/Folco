#include "Interface.hpp"

Interface::Interface(QString hwaddress)
    : HardwareAddress(hwaddress)
{
}

void Interface::addPredefinedIP(PredefinedIP ip)
{
    this->PredefinedIPlist << ip;
}

void Interface::removePredefinedIP(PredefinedIP ip)
{
    for (int i = 0; i < this->PredefinedIPlist.size(); i++) {
        // Read the current predefined IP
        PredefinedIP TmpIP(this->PredefinedIPlist.at(i));

        // Need exact match
        if ((TmpIP.ipAddress() == ip.ipAddress()) && (TmpIP.networkMask() == ip.networkMask()) && (TmpIP.gateway() == ip.gateway())) {
            this->PredefinedIPlist.removeAt(i);

            // Remove only one instance
            break;
        }
    }
}

QString Interface::hardwareAddress() const
{
    return this->HardwareAddress;
}

QList<PredefinedIP> Interface::predefinedIPlist() const
{
    return this->PredefinedIPlist;
}

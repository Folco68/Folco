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
        if (this->predefinedIPlist().at(i) == ip) {
            this->PredefinedIPlist.removeAt(i);
            // Remove only one instance
            break;
        }
    }
}

int Interface::predefinedIPcount() const
{
    return this->PredefinedIPlist.count();
}

QString Interface::hardwareAddress() const
{
    return this->HardwareAddress;
}

QList<PredefinedIP> Interface::predefinedIPlist() const
{
    return this->PredefinedIPlist;
}

void Interface::clearContent()
{
    this->PredefinedIPlist.clear();
}

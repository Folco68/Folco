#include "IFCfg.hpp"

IFCfg::IFCfg(QString HardwareAddress)
    : HardwareAddress(HardwareAddress)
{
}

void IFCfg::addCfg(QString IPaddress, QString NetworkMask, QString Gateway)
{
    QList<QString> Entry;
    Entry << IPaddress << NetworkMask << Gateway;
    this->Configuration << Entry;
}

void IFCfg::removeCfg(QString IPaddress, QString NetworkMask, QString Gateway)
{
    for (int i = 0; i < this->Configuration.size(); i++) {
        QList<QString> Entry;
        Entry = this->Configuration.at(i);
        if ((Entry.at(0) == IPaddress) && (Entry.at(1) == NetworkMask) && (Entry.at(2) == Gateway)) {
            this->Configuration.removeAt(i);
            break; // Remove only one instance
        }
    }
}

QString IFCfg::hardwareAddress() const
{
    return this->HardwareAddress;
}

QList<QList<QString>> IFCfg::configuration() const
{
    return Configuration;
}

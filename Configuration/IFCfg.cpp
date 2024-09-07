#include "IFCfg.hpp"

IFCfg::IFCfg(QString HardwareAddress)
    : HardwareAddress(HardwareAddress)
{
}

void IFCfg::addCfg(QString IPaddress, QString NetworkMask, QString Gateway)
{
    QVector<QString> Entry;
    Entry << IPaddress << NetworkMask << Gateway;
    this->Configuration << Entry;
}

void IFCfg::removeCfg(QString IPaddress, QString NetworkMask, QString Gateway)
{
    for (int i = 0; i < this->Configuration.size(); i++) {
        QVector<QString> Entry;
        Entry = this->Configuration[i];
        if ((Entry[0] == IPaddress) && (Entry[1] == NetworkMask) && (Entry[2] == Gateway))
            this->Configuration.removeAt(i);
        break; // Remove only one instance
    }
}

QString IFCfg::hardwareAddress() const
{
    return this->HardwareAddress;
}

QList<QVector<QString>> IFCfg::configuration() const
{
    return Configuration;
}

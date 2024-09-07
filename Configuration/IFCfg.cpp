#include "IFCfg.hpp"

IFCfg::IFCfg(QString HardwareAddress)
    : HardwareAddress(HardwareAddress)
{
}

void IFCfg::addCfg(QString IPaddress, QString NetworkMask, QString Gateway)
{
    this->IPaddress.append(IPaddress);
    this->NetworkMask.append(NetworkMask);
    this->Gateway.append(Gateway);
}

void IFCfg::removeCfg(QString IPaddress, QString NetworkMask, QString Gateway)
{
    for (int i = 0; i < this->IPaddress.size(); i++) {
        if ((this->IPaddress.at(i) == IPaddress) && (this->NetworkMask.at(i) == NetworkMask) && (this->Gateway.at(i) == Gateway)) {
            this->IPaddress.removeAt(i);
            this->NetworkMask.removeAt(i);
            this->Gateway.removeAt(i);
            break; // Remove only one instance
        }
    }
}

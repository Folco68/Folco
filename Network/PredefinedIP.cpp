#include "PredefinedIP.hpp"

PredefinedIP::PredefinedIP(QString ip, QString mask, QString gateway)
    : IPaddress(ip)
    , NetworkMask(mask)
    , Gateway(gateway)
{
}

QString PredefinedIP::ipAddress() const
{
    return this->IPaddress;
}

QString PredefinedIP::networkMask() const
{
    return this->NetworkMask;
}

QString PredefinedIP::gateway() const
{
    return this->Gateway;
}

bool PredefinedIP::isValid() const
{
    return !this->IPaddress.isNull();
}

void PredefinedIP::setPredefinedIP(QString ip, QString mask, QString gateway)
{
    this->IPaddress   = ip;
    this->NetworkMask = mask;
    this->Gateway     = gateway;
}

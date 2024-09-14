#include "PredefinedIP.hpp"

PredefinedIP::PredefinedIP(QString Name, QString ip, QString mask, QString gateway)
    : Name(Name)
    , IPaddress(ip)
    , NetworkMask(mask)
    , Gateway(gateway)
{
}

QString PredefinedIP::name() const
{
    return this->Name;
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

void PredefinedIP::setPredefinedIP(QString Name, QString ip, QString mask, QString gateway)
{
    this->Name        = Name;
    this->IPaddress   = ip;
    this->NetworkMask = mask;
    this->Gateway     = gateway;
}

bool PredefinedIP::operator==(PredefinedIP ip) const
{
    return (this->Name == ip.Name) && (this->IPaddress == ip.IPaddress) && (this->NetworkMask == ip.NetworkMask) && (this->Gateway == ip.Gateway);
}

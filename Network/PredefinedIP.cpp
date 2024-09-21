#include "PredefinedIP.hpp"

PredefinedIP::PredefinedIP(Interface* parent, QString Name, QString ip, QString mask, QString gateway)
    : Parent(parent)
    , Name(Name)
    , IPaddress(ip)
    , NetworkMask(mask)
    , Gateway(gateway)
{
}

Interface* PredefinedIP::parent() const
{
    return this->Parent;
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

bool PredefinedIP::operator==(PredefinedIP ip) const
{
    return (this->Parent == ip.Parent) && (this->Name == ip.Name) && (this->IPaddress == ip.IPaddress) && (this->NetworkMask == ip.NetworkMask) && (this->Gateway == ip.Gateway);
}

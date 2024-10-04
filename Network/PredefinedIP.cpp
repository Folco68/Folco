#include "PredefinedIP.hpp"
#include <QByteArray>

PredefinedIP::PredefinedIP(Interface* parent, QString Name, QString ip, QString mask, QString gateway)
    : Parent(parent)
    , Name(Name)
    , IPaddress(ip)
    , NetworkMask(mask)
    , Gateway(gateway)
{
}

PredefinedIP::PredefinedIP(Interface* parent, QDataStream& stream)
    : Parent(parent)
{
    QByteArray Name;
    QByteArray IPaddress;
    QByteArray NetworkMask;
    QByteArray GateWay;

    stream >> Name >> IPaddress >> NetworkMask >> GateWay;
    this->Name        = QString::fromUtf8(Name);
    this->IPaddress   = QString::fromUtf8(IPaddress);
    this->NetworkMask = QString::fromUtf8(NetworkMask);
    this->Gateway     = QString::fromUtf8(GateWay);
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

void PredefinedIP::save(QDataStream& stream)
{
    stream << this->Name.toUtf8() << this->IPaddress.toUtf8() << this->NetworkMask.toUtf8() << this->Gateway.toUtf8();
}

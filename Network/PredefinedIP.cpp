/**************************************************************************************** 
 *                                                                                      * 
 *     Folco - Program allowing to quickly change the IPv4 address of an interface      * 
 *                       Copyright (C) 2024-2025 Martial Demolins                       * 
 *                                                                                      * 
 *         This program is free software: you can redistribute it and/or modify         * 
 *         it under the terms of the GNU General Public License as published by         * 
 *          the Free Software Foundation, either version 3 of the License, or           * 
 *                          (at your option) any later version                          * 
 *                                                                                      * 
 *            This program is distributed in the hope that it will be useful            * 
 *            but WITHOUT ANY WARRANTY; without even the implied warranty of            * 
 *            MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             * 
 *                     GNU General Public License for more details                      * 
 *                                                                                      * 
 *          You should have received a copy of the GNU General Public License           * 
 *         along with this program.  If not, see <https://www.gnu.org/licenses>         * 
 *                                                                                      * 
 ****************************************************************************************/

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
    return (this->Parent == ip.Parent) && (this->Name == ip.Name) && (this->IPaddress == ip.IPaddress) && (this->NetworkMask == ip.NetworkMask)
        && (this->Gateway == ip.Gateway);
}

void PredefinedIP::save(QDataStream& stream)
{
    stream << this->Name.toUtf8() << this->IPaddress.toUtf8() << this->NetworkMask.toUtf8() << this->Gateway.toUtf8();
}

bool PredefinedIP::hasNetworkMask() const
{
    return this->NetworkMask != QString("0.0.0.0");
}

bool PredefinedIP::hasGateway() const
{
    return this->Gateway != QString("0.0.0.0");
}

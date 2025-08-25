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

#include "Configuration.hpp"
#include <QByteArray>

Configuration::Configuration(QString hwaddress, QString customname, QString name)
    : HardwareAddress(hwaddress)
    , CustomName(customname)
    , HumanReadableName(name)
{
}

Configuration::Configuration(QDataStream& stream, qint32 version)
{
    if (version == 1) {
        // Read HW address and name
        QByteArray HardwareAddress;
        QByteArray HumanReadableName;
        stream >> HardwareAddress >> HumanReadableName;
        this->HardwareAddress   = QString::fromUtf8(HardwareAddress);
        this->HumanReadableName = QString::fromUtf8(HumanReadableName);

        // Read the number of predefined IP
        qint32 Count;
        stream >> Count;

        // Read the predefined IP
        for (int i = 0; i < Count; i++) {
            PredefinedIP* IP = new PredefinedIP(stream);
            this->PredefinedIPlist.append(IP);
        }
    }
    else if (version == 2) {
        // Read HW address and name
        QByteArray HardwareAddress;
        QByteArray CustomName;
        QByteArray HumanReadableName;
        stream >> HardwareAddress >> CustomName >> HumanReadableName;
        this->HardwareAddress   = QString::fromUtf8(HardwareAddress);
        this->CustomName        = QString::fromUtf8(CustomName);
        this->HumanReadableName = QString::fromUtf8(HumanReadableName);

        // Read the number of predefined IP
        qint32 Count;
        stream >> Count;

        // Read the predefined IP
        for (int i = 0; i < Count; i++) {
            PredefinedIP* IP = new PredefinedIP(stream);
            this->PredefinedIPlist.append(IP);
        }
    }
}

Configuration::~Configuration()
{
    for (int i = 0; i < this->PredefinedIPlist.count(); i++) {
        delete this->PredefinedIPlist.at(i);
    }
}

void Configuration::addPredefinedIP(PredefinedIP* ip)
{
    this->PredefinedIPlist << ip;
}

int Configuration::predefinedIPcount() const
{
    return this->PredefinedIPlist.count();
}

bool Configuration::hasPredefinedIP() const
{
    return !this->PredefinedIPlist.isEmpty();
}

QString Configuration::hardwareAddress() const
{
    return this->HardwareAddress;
}

QList<PredefinedIP*> Configuration::predefinedIPlist() const
{
    return this->PredefinedIPlist;
}

void Configuration::clearContent()
{
    this->CustomName.clear();
    while (!this->PredefinedIPlist.isEmpty()) {
        delete this->PredefinedIPlist.takeLast();
    }
}

void Configuration::save(QDataStream& stream)
{
    stream << this->HardwareAddress.toUtf8() << this->CustomName.toUtf8() << this->HumanReadableName.toUtf8();
    qint32 Count = this->predefinedIPcount();
    stream << Count;
    for (int i = 0; i < Count; i++) {
        this->PredefinedIPlist.at(i)->save(stream);
    }
}

QString Configuration::humanReadableName() const
{
    return this->HumanReadableName;
}

void Configuration::setHumanReadableName(QString name)
{
    this->HumanReadableName = name;
}

QString Configuration::customName() const
{
    return this->CustomName;
}

void Configuration::setCustomName(QString name)
{
    this->CustomName = name;
}

/*******************************************************************************
 *                                                                             *
 * Folco - Program allowing to quickly change the IPv4 address of an interface *
 *                     Copyright (C) 2024 Martial Demolins                     *
 *                                                                             *
 *    This program is free software: you can redistribute it and/or modify     *
 *    it under the terms of the GNU General Public License as published by     *
 *      the Free Software Foundation, either version 3 of the License, or      *
 *                      at your option) any later version                      *
 *                                                                             *
 *       This program is distributed in the hope that it will be useful        *
 *       but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
 *                 GNU General Public License for more details                 *
 *                                                                             *
 *      You should have received a copy of the GNU General Public License      *
 *     along with this program.  If not, see <https://www.gnu.org/licenses     *
 *                                                                             *
 ******************************************************************************/

#include "Interface.hpp"
#include <QByteArray>

Interface::Interface(QString hwaddress, QString name)
    : HardwareAddress(hwaddress)
    , HumanReadableName(name)
{
}

Interface::Interface(QDataStream& stream)
{
    // Read HW address and name
    QByteArray HardwareAddress;
    QByteArray HumanReadableName;
    stream >> HardwareAddress >> HumanReadableName;
    this->HardwareAddress = QString::fromUtf8(HardwareAddress);
    this->HumanReadableName = QString::fromUtf8(HumanReadableName);

    // Read the number of predefined IP
    qint32 Count;
    stream >> Count;

    // Read the predefined IP
    for (int i = 0; i < Count; i++) {
        PredefinedIP* IP = new PredefinedIP(this, stream);
        this->PredefinedIPlist.append(IP);
    }
}

Interface::~Interface()
{
    for (int i = 0; i < this->PredefinedIPlist.count(); i++) {
        delete this->PredefinedIPlist.at(i);
    }
}

void Interface::addPredefinedIP(PredefinedIP* ip)
{
    this->PredefinedIPlist << ip;
}

void Interface::removePredefinedIP(PredefinedIP* ip)
{
    for (int i = 0; i < this->PredefinedIPlist.size(); i++) {
        if (*(this->predefinedIPlist().at(i)) == *ip) {
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

QList<PredefinedIP*> Interface::predefinedIPlist() const
{
    return this->PredefinedIPlist;
}

void Interface::clearContent()
{
    this->PredefinedIPlist.clear();
}

void Interface::save(QDataStream& stream)
{
    stream << this->HardwareAddress.toUtf8() << this->HumanReadableName.toUtf8();
    qint32 Count = this->predefinedIPcount();
    stream << Count;
    for (int i = 0; i < Count; i++) {
        this->PredefinedIPlist.at(i)->save(stream);
    }
}

QString Interface::humanReadableName() const
{
    return this->HumanReadableName;
}

void Interface::setHumanReadableName(QString name)
{
    this->HumanReadableName = name;
}

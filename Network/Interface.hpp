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

#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include "PredefinedIP.hpp"
#include <QDataStream>
#include <QList>
#include <QString>

class Interface
{
  public:
    Interface(QString hwaddress, QString name);
    Interface(QDataStream& stream);
    ~Interface();
    void                 addPredefinedIP(PredefinedIP* ip);
    void                 removePredefinedIP(PredefinedIP* ip);
    int                  predefinedIPcount() const;
    QString              hardwareAddress() const;
    QList<PredefinedIP*> predefinedIPlist() const;
    void                 clearContent();
    void                 save(QDataStream& stream);
    QString              humanReadableName() const;
    void                 setHumanReadableName(QString name);

  private:
    QString              HardwareAddress;
    QString              HumanReadableName; // This name can be changed by the user in the OS UI.
                                            // It is only used to display the Interface related PredefinedIP
                                            // when its hardware device is disconnected
    QList<PredefinedIP*> PredefinedIPlist;
};

#endif // INTERFACE_HPP

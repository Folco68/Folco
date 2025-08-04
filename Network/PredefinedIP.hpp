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

#ifndef PREDEFINED_IP_HPP
#define PREDEFINED_IP_HPP

#include <QDataStream>
#include <QString>

class Interface;

class PredefinedIP
{
  public:
    PredefinedIP(Interface* parent, QString Name, QString ip, QString mask, QString gateway);
    PredefinedIP(Interface* parent, QDataStream& stream);
    Interface* parent() const;
    QString    name() const;
    QString    ipAddress() const;
    QString    networkMask() const;
    QString    gateway() const;
    bool       operator==(PredefinedIP ip) const;
    void       save(QDataStream& stream);
    bool       hasNetworkMask() const;
    bool       hasGateway() const;

  private:
    // Keep a track of the interface which own this settings, to find the interface to configure when this Predefined IP is selected in the tray icon context menu
    Interface* Parent;
    QString    Name;
    QString    IPaddress;
    QString    NetworkMask;
    QString    Gateway;
};

#endif // PREDEFINED_IP_HPP

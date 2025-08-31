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

class Configuration;

class PredefinedIP
{
  public:
    PredefinedIP(QString Name, QString ip, QString mask, QString gateway); // "Manual" creation of a PDI
    PredefinedIP(QDataStream& stream);                                     // Used on startup, when reading data file
    PredefinedIP(PredefinedIP* PDI);                                       // Used when merging 2 Configurations

    bool operator==(PredefinedIP ip) const; // Deep comparison, used when merging 2 Configurations
    void save(QDataStream& stream);         // Save in the data file. Called on exit

    QString name() const;        //
    QString ipAddress() const;   // Getters
    QString networkMask() const; //
    QString gateway() const;     //

  private:
    QString Name;        // User-defined name, to easily remember the purpose of the IP
    QString IPaddress;   //
    QString NetworkMask; // IPv4 stuff...
    QString Gateway;     //
};

#endif // PREDEFINED_IP_HPP

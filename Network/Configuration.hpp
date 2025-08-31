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

#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include "PredefinedIP.hpp"
#include <QDataStream>
#include <QList>
#include <QString>

/********************************************************************************************************************** 
 *                                                                                                                    * 
 *                                                   Configuration                                                    * 
 *                                                                                                                    * 
 *                         This class holds the predefined IPs linked to a hardware interface                         * 
 *                                                                                                                    * 
 **********************************************************************************************************************/

class Configuration
{
  public:
    Configuration(QString hwaddress, QString customname, QString name); // Create a Configuration on user request
    Configuration(QDataStream& stream, qint32 version);                 // Create all saved Configurations at startup
    ~Configuration();

    void addPredefinedIP(PredefinedIP* ip); // Add one Predefined IP
    void clearContent();                    // Delete all the Predefined IP and the custom name
    void save(QDataStream& stream);         // Save the Configuration into a file
    void setHumanReadableName(QString name);
    void setCustomName(QString name);

    int  predefinedIPcount() const;                // Return the Predefined IP count
    bool hasPredefinedIP() const;                  // Return true if the Configuration holds at least one Predefined IP
    bool hasPredefinedIP(PredefinedIP* PDI) const; // Return true if PDI is held by this Configuration

    QString              hardwareAddress() const;   // Return the MAC address to which the Configuration is linked
    QString              customName() const;        // User-defined name, more suitable (Office Workstation, USB Adapter, ...)
    QString              humanReadableName() const; // Name given by the OS (Ethernet, Ethernet2, Ethernet3, VMWare Network Adapter, ...)
    QList<PredefinedIP*> predefinedIPlist() const;  // Return the Predefined IP list

  private:
    QString              HardwareAddress;   // MAC address
    QString              CustomName;        // Not mandatory, not OS-related. The scope is this application, as a reminder
    QString              HumanReadableName; // This name can be changed by the user in the OS UI.
                                            // It is used as a reminder when the Network Interface is disconnected
    QList<PredefinedIP*> PredefinedIPlist;
};

#endif // CONFIGURATION_HPP

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
    Configuration(QString hwaddress, QString customname, QString name);
    Configuration(QDataStream& stream, qint32 version);
    ~Configuration();
    void                 addPredefinedIP(PredefinedIP* ip);
    int                  predefinedIPcount() const;
    bool                 hasPredefinedIP() const;
    bool                 hasPredefinedIP(PredefinedIP* PDI) const;
    QString              hardwareAddress() const;
    QList<PredefinedIP*> predefinedIPlist() const;
    void                 clearContent();
    void                 save(QDataStream& stream);
    QString              humanReadableName() const;          // Name given par the OS (Ethernet, Ethernet2, Ethernet3, VMWare Network Adapter, ...)
    void                 setHumanReadableName(QString name); //
    QString              customName() const;                 // More suitable name (Office Workstation, USB Adapter, ...)
    void                 setCustomName(QString name);

  private:
    QString              HardwareAddress;   // MAC address
    QString              CustomName;        // Not mandatory, not OS-related. The scope is this application, as a reminder
    QString              HumanReadableName; // This name can be changed by the user in the OS UI.
                                            // It is only used to display the Configuration related PredefinedIP when its hardware device is disconnected
    QList<PredefinedIP*> PredefinedIPlist;
};

#endif // CONFIGURATION_HPP

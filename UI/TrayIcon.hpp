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

#ifndef TRAYICON_HPP
#define TRAYICON_HPP

#include "../Network/PredefinedIP.hpp"
#include <QMenu>
#include <QPoint>
#include <QSystemTrayIcon>

class TrayIcon: public QSystemTrayIcon
{
    Q_OBJECT

  public:
    explicit TrayIcon();
    ~TrayIcon();

  private:
    QMenu* ContextMenu;

    void showContextMenu(QPoint position);                               // Triggered when the icon of the System Tray is clicked
    void configureInterfacePredefinedIP(QString name, PredefinedIP* ip); // Set a IPv4 address to a Network Interface
    void configureInterfaceDHCP(QString name);                           // Set a Network Interface to DHCP
};

#endif // TRAYICON_HPP

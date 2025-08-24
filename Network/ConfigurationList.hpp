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

#ifndef CONFIGURATION_LIST_HPP
#define CONFIGURATION_LIST_HPP

#include "Configuration.hpp"
#include "PredefinedIP.hpp"
#include <QDataStream>
#include <QList>
#include <QString>

/********************************************************************************************************************** 
 *                                                                                                                    * 
 *                                                 ConfigurationList                                                  * 
 *                                                                                                                    * 
 *                This class contains a list of all the interfaces showed when opening the tray menu.                 * 
 *                                                                                                                    * 
 **********************************************************************************************************************/

class ConfigurationList
{
  public:
    static ConfigurationList* instance();
    static void           release();

    Configuration*        configuration(QString hwaddress) const;
    bool                  hasPredefinedIP(QString hwaddress) const;
    void                  addConfiguration(Configuration* configuration);
    void                  deleteConfiguration(Configuration* configuration);
    QList<Configuration*> configurationList() const;

  private:
    // Singleton stuff
    static ConfigurationList* configurationlist;
    ConfigurationList();
    ~ConfigurationList();

    void open();
    bool save() const;

    QList<Configuration*> List;
};

#endif // CONFIGURATION_LIST_HPP

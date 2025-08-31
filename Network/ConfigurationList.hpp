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
 *                          This class holds and manage the list of all the Configurations                            * 
 *                                                                                                                    * 
 **********************************************************************************************************************/

class ConfigurationList
{
    //--------------------------------------------------------------------------
    //                             Singleton stuff                             -
    //--------------------------------------------------------------------------

  public:
    static ConfigurationList* instance();
    static void               release();

  private:
    static ConfigurationList* configurationlist;
    ConfigurationList();
    ~ConfigurationList();

    //--------------------------------------------------------------------------
    //                            ConfigurationList                            -
    //--------------------------------------------------------------------------

  public:
    Configuration*        configuration(QString hwaddress) const;            // Return a pointer to a Configuration or nullptr, starting from a MAC address
    void                  addConfiguration(Configuration* configuration);    // Add a Configuration to the list
    void                  deleteConfiguration(Configuration* configuration); // Delete a Configuration from the list
    QList<Configuration*> configurationList() const;                         // Return the list itself

  private:
    void open();       // Open the data file on start-up
    bool save() const; // Save the data file on exit

    QList<Configuration*> List;
};

#endif // CONFIGURATION_LIST_HPP

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

#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <QSettings>

class Settings: public QSettings
{
  public:
    static Settings* instance();
    static void      release();

    bool showOnlyEthernetWifi() const;
    void setShowOnlyEthernetWifi(bool enabled);

    bool showOnlyUp() const;
    void setShowOnlyUp(bool enabled);

    bool showOnlyPredefined() const;
    void setShowOnlyPredefined(bool enabled);

  private:
    static Settings* settings;
    Settings();
    ~Settings();
    void logSetting(QString name, bool enabled) const;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                                                                                             ///
///                                                 Key names and default values                                                ///
///                                                                                                                             ///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define KEY_SHOW_ONLY_ETHERNET_WIFI     "ShowOnlyEthernetWiFi"
#define DEFAULT_SHOW_ONLY_ETHERNET_WIFI true

#define KEY_SHOW_ONLY_UP     "ShowOnlyUp"
#define DEFAULT_SHOW_ONLY_UP false

#define KEY_SHOW_ONLY_PREDEFINED     "ShowOnlyPredefined"
#define DEFAULT_SHOW_ONLY_PREDEFINED false


#endif // SETTINGS_HPP

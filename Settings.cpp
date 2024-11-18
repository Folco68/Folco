/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Folco - Program allowing to quickly change an IPv4 on an interface      //
// Copyright (C) 2024 Martial Demolins                                     //
//                                                                         //
// This program is free software: you can redistribute it and/or modify    //
// it under the terms of the GNU General Public License as published by    //
// the Free Software Foundation, either version 3 of the License, or       //
// (at your option) any later version.                                     //
//                                                                         //
// This program is distributed in the hope that it will be useful,         //
// but WITHOUT ANY WARRANTY; without even the implied warranty of          //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           //
// GNU General Public License for more details.                            //
//                                                                         //
// You should have received a copy of the GNU General Public License       //
// along with this program.  If not, see <https://www.gnu.org/licenses/>.  //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

#include "Settings.hpp"
#include "Global.hpp"
#include "Logger.hpp"

Settings* Settings::settings = nullptr;

Settings::Settings()
    : QSettings(ORGANIZATION_NAME, APPLICATION_NAME)
{
}

Settings::~Settings()
{
}

Settings* Settings::instance()
{
    if (settings == nullptr) {
        settings = new Settings();
    }
    return settings;
}

void Settings::release()
{
    if (settings != nullptr) {
        delete settings;
    }
    settings = nullptr;
}

bool Settings::showOnlyEthernetWifi()
{
    return value(KEY_SHOW_ONLY_ETHERNET_WIFI, DEFAULT_SHOW_ONLY_ETHERNET_WIFI).toBool();
}

void Settings::setShowOnlyEthernetWifi(bool enabled)
{
    setValue(KEY_SHOW_ONLY_ETHERNET_WIFI, enabled);
    logSetting(KEY_SHOW_ONLY_ETHERNET_WIFI, enabled);
}

bool Settings::showOnlyUp()
{
    return value(KEY_SHOW_ONLY_UP, DEFAULT_SHOW_ONLY_UP).toBool();
}

void Settings::setShowOnlyUp(bool enabled)
{
    setValue(KEY_SHOW_ONLY_UP, enabled);
    logSetting(KEY_SHOW_ONLY_UP, enabled);
}

bool Settings::showOnlyPredefined()
{
    return value(KEY_SHOW_ONLY_PREDEFINED, DEFAULT_SHOW_ONLY_PREDEFINED).toBool();
}

void Settings::setShowOnlyPredefined(bool enabled)
{
    setValue(KEY_SHOW_ONLY_PREDEFINED, enabled);
    logSetting(KEY_SHOW_ONLY_PREDEFINED, enabled);
}

void Settings::logSetting(QString name, bool enabled) const
{
    Logger::instance()->addLogEntry(QString("Settings: %1 set to %2").arg(name, enabled ? "true" : "false"));
}

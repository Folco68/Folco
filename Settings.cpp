#include "Settings.hpp"
#include "Global.hpp"

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
}

bool Settings::showOnlyUp()
{
    return value(KEY_SHOW_ONLY_UP, DEFAULT_SHOW_ONLY_UP).toBool();
}

void Settings::setShowOnlyUp(bool enabled)
{
    setValue(KEY_SHOW_ONLY_UP, enabled);
}

bool Settings::showOnlyPredefined()
{
    return value(KEY_SHOW_ONLY_PREDEFINED, DEFAULT_SHOW_ONLY_PREDEFINED).toBool();
}

void Settings::setShowOnlyPredefined(bool enabled)
{
    setValue(KEY_SHOW_ONLY_PREDEFINED, enabled);
}

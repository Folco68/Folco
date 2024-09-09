#ifndef SETTINGS_HPP
#define SETTINGS_HPP


#include <QSettings>


class Settings: public QSettings
{
  public:
    static Settings* instance();
    static void      release();

    bool showOnlyEthernetWifi();
    void setShowOnlyEthernetWifi(bool enabled);

    bool showOnlyUp();
    void setShowOnlyUp(bool enabled);

    bool showOnlyPredefined();
    void setShowOnlyPredefined(bool enabled);

  private:
    static Settings* settings;
    Settings();
    ~Settings();
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                                                                                             ///
///                                                 Key names and default values                                                ///
///                                                                                                                             ///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define KEY_SHOW_ONLY_ETHERNET_WIFI     "ShowOnlyEthernetWiFi"
#define DEFAULT_SHOW_ONLY_ETHERNET_WIFI false

#define KEY_SHOW_ONLY_UP     "ShowOnlyUp"
#define DEFAULT_SHOW_ONLY_UP false

#define KEY_SHOW_ONLY_PREDEFINED     "ShowOnlyPredefined"
#define DEFAULT_SHOW_ONLY_PREDEFINED false


#endif // SETTINGS_HPP

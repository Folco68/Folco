#ifndef DLGEDITCONFIG_HPP
#define DLGEDITCONFIG_HPP

#include "../Configuration/Configuration.hpp"
#include <QDialog>
#include <QList>
#include <QString>

namespace Ui {
    class DlgEditConfig;
}

class DlgEditConfig: public QDialog
{
    Q_OBJECT

  public:
    static Configuration dlgNewConfig(QWidget* Parent);
    static bool          dlgEditConfig(QWidget* Parent, Configuration Config);

  private:
    Ui::DlgEditConfig* ui;
    Configuration      Config;
    DlgEditConfig(QWidget* parent, Configuration Config = Configuration());
    ~DlgEditConfig();
};

#endif // DLGEDITCONFIG_HPP

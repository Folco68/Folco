#ifndef DLGEDITCONFIG_HPP
#define DLGEDITCONFIG_HPP

#include "../Configuration/PredefinedIP.hpp"
#include <QDialog>

namespace Ui {
    class DlgEditConfig;
}

class DlgEditConfig: public QDialog
{
    Q_OBJECT

  public:
    static PredefinedIP dlgNewConfig(QWidget* parent);
    static bool         dlgEditConfig(QWidget* parent, PredefinedIP ip);

  private:
    Ui::DlgEditConfig* ui;
    PredefinedIP       IP;
    DlgEditConfig(QWidget* parent, PredefinedIP ip = PredefinedIP());
    ~DlgEditConfig();
};

#endif // DLGEDITCONFIG_HPP

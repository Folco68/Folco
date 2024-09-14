#ifndef DLGSETTINGS_HPP
#define DLGSETTINGS_HPP

#include <QDialog>

namespace Ui {
    class DlgSettings;
}

class DlgSettings: public QDialog
{
    Q_OBJECT

  public:
    static void execDlgSettings();

  private:
    explicit DlgSettings();
    ~DlgSettings();

    Ui::DlgSettings* ui;
};

#endif // DLGSETTINGS_HPP

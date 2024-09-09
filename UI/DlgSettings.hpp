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
    static int execDlgSettings();

  private:
    explicit DlgSettings(QWidget* parent = nullptr);
    ~DlgSettings();

    Ui::DlgSettings* ui;
};

#endif // DLGSETTINGS_HPP

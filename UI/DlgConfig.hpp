#ifndef DLGCONFIG_HPP
#define DLGCONFIG_HPP

#include <QDialog>

namespace Ui {
    class DlgConfig;
}

class DlgConfig: public QDialog
{
    Q_OBJECT

  public:
    static int execDlgConfig();

  private:
    explicit DlgConfig(QWidget* parent = nullptr);
    ~DlgConfig();

    Ui::DlgConfig* ui;

    void refreshInterfaces();
};

typedef enum {
    COLUMN_NAME,
    COLUMN_LONG_NAME,
    COLUMN_TYPE,
    COLUMN_IP_ADDRESS,
    COLUMN_NETWORK_MASK,
    COLUMN_HARDWARE_ADDRESS
} COLUMN_TITLES;

#endif // DLGCONFIG_HPP

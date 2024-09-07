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

    void refreshInterface();
    void interfaceSelectionChanged();
    void addConfiguration();
};

typedef enum {
    COLUMN_IF_NAME,
    COLUMN_IF_IP_ADDRESS,
    COLUMN_IF_NETWORK_MASK,
    COLUMN_IF_HARDWARE_ADDRESS
} COLUMN_INTERFACES;

typedef enum {
    COLUMN_CONFIG_IP_ADDRESS,
    COLUMN_CONFIG_NETWORK_MASK,
    COLUMN_CONFIG_GATEWAY
} COLUMN_CONFIG;


// Add some extra space in the columns to make the view more clear
#define COLUMN_EXTRA_SIZE 30

#endif // DLGCONFIG_HPP

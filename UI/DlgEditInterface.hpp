#ifndef DLG_EDIT_INTERFACE_HPP
#define DLG_EDIT_INTERFACE_HPP

#include "../Network/Interface.hpp"
#include <QDialog>
#include <QNetworkInterface>

namespace Ui {
    class DlgEditInterface;
}

class DlgEditInterface: public QDialog
{
    Q_OBJECT

  public:
    static void execDlgEditInterface(QNetworkInterface NetworkInterface);
    ~DlgEditInterface();

  private:
    explicit DlgEditInterface(QNetworkInterface NetworkInterface);
    void tableSelectionChanged();
    void newPredefinedIP();
    void editPredefinedIP();
    void deletePredefinedIP();

    void writeContent(Interface* interface);

    Ui::DlgEditInterface* ui;
};

typedef enum {
    COLUMN_NAME,
    COLUMN_IP_ADDRESS,
    COLUMN_NETWORK_MASK,
    COLUMN_GATEWAY
} COLUMN_HEADER;

#endif // DLG_EDIT_INTERFACE_HPP

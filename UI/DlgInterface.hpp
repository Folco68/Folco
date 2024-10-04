#ifndef DLG_INTERFACE_HPP
#define DLG_INTERFACE_HPP

#include "../Network/Interface.hpp"
#include <QDialog>
#include <QNetworkInterface>

namespace Ui {
    class DlgInterface;
}

// This dialog uses the list of PredefinedIP of an Interface to fill its table.
// Only the execDlgInterface really create and delete PredefinedIP
// All other functions just modify the tables
class DlgInterface: public QDialog
{
    Q_OBJECT

  public:
    static void execDlgInterface(QNetworkInterface NetworkInterface);
    ~DlgInterface();

  private:
    explicit DlgInterface(QNetworkInterface NetworkInterface);
    void tableSelectionChanged();
    void newPredefinedIP();
    void editPredefinedIP();
    void deletePredefinedIP();

    void writeContent(Interface* interface);

    Ui::DlgInterface* ui;
};

typedef enum {
    COLUMN_NAME,
    COLUMN_IP_ADDRESS,
    COLUMN_NETWORK_MASK,
    COLUMN_GATEWAY
} COLUMN_HEADER;

#endif // DLG_INTERFACE_HPP

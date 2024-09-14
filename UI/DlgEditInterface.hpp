#ifndef DLG_EDIT_INTERFACE_HPP
#define DLG_EDIT_INTERFACE_HPP

#include <QDialog>
#include <QString>

namespace Ui {
    class DlgEditInterface;
}

class DlgEditInterface: public QDialog
{
    Q_OBJECT

  public:
    static void execDlgEditInterface(QString hwaddress);
    ~DlgEditInterface();

  private:
    explicit DlgEditInterface(QWidget* parent = nullptr);
    Ui::DlgEditInterface* ui;
};

#endif // DLG_EDIT_INTERFACE_HPP

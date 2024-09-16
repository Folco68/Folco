#ifndef DLGEDITPREDEFINEDIP_HPP
#define DLGEDITPREDEFINEDIP_HPP

#include <QDialog>

namespace Ui {
    class DlgEditPredefinedIP;
}

class DlgEditPredefinedIP: public QDialog
{
    Q_OBJECT

  public:
    static int newPredefinedIP(QWidget* parent, QString* name, QString* ip, QString* networkmask, QString* gateway);
    static int editPredefinedIP(QWidget* parent, QString* name, QString* ip, QString* networkmask, QString* gateway);

  private:
    explicit DlgEditPredefinedIP(QWidget* parent, QString* name = nullptr, QString* ip = nullptr, QString* networkmask = nullptr, QString* gateway = nullptr);
    ~DlgEditPredefinedIP();

    Ui::DlgEditPredefinedIP* ui;
};

#endif // DLGEDITPREDEFINEDIP_HPP

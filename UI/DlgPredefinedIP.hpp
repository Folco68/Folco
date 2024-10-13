#ifndef DLG_PREDEFINED_IP_HPP
#define DLG_PREDEFINED_IP_HPP

#include <QDialog>

namespace Ui {
    class DlgPredefinedIP;
}

class DlgPredefinedIP: public QDialog
{
    Q_OBJECT

  public:
    static int newPredefinedIP(QWidget* parent, QString* name = nullptr, QString* ip = nullptr, QString* networkmask = nullptr, QString* gateway = nullptr);
    static int editPredefinedIP(QWidget* parent, QString* name, QString* ip, QString* networkmask, QString* gateway);

  private:
    explicit DlgPredefinedIP(QWidget* parent, QString* name = nullptr, QString* ip = nullptr, QString* networkmask = nullptr, QString* gateway = nullptr);
    ~DlgPredefinedIP();

    Ui::DlgPredefinedIP* ui;
};

#endif // DLG_PREDEFINED_IP_HPP

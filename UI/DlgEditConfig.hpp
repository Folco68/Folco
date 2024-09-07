#ifndef DLGEDITCONFIG_HPP
#define DLGEDITCONFIG_HPP

#include <QDialog>
#include <QList>
#include <QString>

namespace Ui {
    class DlgEditConfig;
}

class DlgEditConfig: public QDialog
{
    Q_OBJECT

  public:
    static QList<QString> dlgNewConfig(QWidget* Parent);
    static bool           dlgEditConfig(QWidget* Parent, QList<QString> Config);

  private:
    Ui::DlgEditConfig* ui;
    QList<QString>     Configuration;
    DlgEditConfig(QWidget* parent, QList<QString> Configuration = QList<QString>());
    ~DlgEditConfig();
};

#endif // DLGEDITCONFIG_HPP

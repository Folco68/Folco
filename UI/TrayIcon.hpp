#ifndef TRAYICON_HPP
#define TRAYICON_HPP

#include "../Network/PredefinedIP.hpp"
#include <QMenu>
#include <QSystemTrayIcon>

class TrayIcon: public QSystemTrayIcon
{
    Q_OBJECT

  public:
    explicit TrayIcon();
    ~TrayIcon();

  private:
    void   showContextMenu();
    QMenu* ContextMenu;
    void   configureInterface(QString name, PredefinedIP ip);
    void   configureInterfaceDHCP(QString name);
};

#endif // TRAYICON_HPP

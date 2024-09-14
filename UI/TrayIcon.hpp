#ifndef TRAYICON_HPP
#define TRAYICON_HPP

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
};

#endif // TRAYICON_HPP

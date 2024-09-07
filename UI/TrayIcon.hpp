#ifndef TRAYICON_HPP
#define TRAYICON_HPP

#include <QAction>
#include <QMenu>
#include <QSystemTrayIcon>

class TrayIcon: public QSystemTrayIcon
{
    Q_OBJECT

  public:
    explicit TrayIcon();
};

#endif // TRAYICON_HPP

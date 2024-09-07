#include "TrayIcon.hpp"
#include "DlgConfig.hpp"
#include <QCoreApplication>
#include <QCursor>
#include <QIcon>

TrayIcon::TrayIcon()
    : QSystemTrayIcon {QIcon(":/Icons/IconBase.png")}
{
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///                                                                                                                             ///
    ///                                          Create the base of the tray icon context menu                                      ///
    ///                                                                                                                             ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    QMenu* ContextMenu = new QMenu;

    // Create actions. Give the parentality to ContextMenu, avoiding to destroy items by hand on exit
    QAction* ActionConfigure = new QAction("Configure...", ContextMenu);
    QAction* ActionAbout     = new QAction("About...", ContextMenu);
    QAction* ActionExit      = new QAction("Exit", ContextMenu);

    // Populate the context menu
    ContextMenu->addSeparator();
    ContextMenu->addAction(ActionConfigure);
    ContextMenu->addAction(ActionAbout);
    ContextMenu->addSeparator();
    ContextMenu->addAction(ActionExit);

    // Add the context menu to the tray icon
    setContextMenu(ContextMenu);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///                                                                                                                             ///
    ///                                                     Set base connections                                                    ///
    ///                                                                                                                             ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Show the context menu regardless of the trigger (default: only the right click displays the menu)
    connect(this, &QSystemTrayIcon::activated, this, [ContextMenu]() { ContextMenu->popup(QCursor::pos()); });

    // Configuration dialog
    connect(ActionConfigure, &QAction::triggered, []() { DlgConfig::execDlgConfig(); });

    // Exit the application
    connect(ActionExit, &QAction::triggered, []() { QCoreApplication::exit(0); });
}

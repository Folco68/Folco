#include "UI/TrayIcon.hpp"
#include <QApplication>

int main(int argc, char* argv[])
{
    // Create and set up the application
    // We don't want the application to exit when a window is closed
    QApplication Application(argc, argv);
    Application.setQuitOnLastWindowClosed(false);

    // Create and install the tray icon
    TrayIcon* TrayIcon = new class TrayIcon;
    TrayIcon->show();

    // Start the event loop
    int RetVal = Application.exec();

    // Clean and quit
    delete TrayIcon;
    return RetVal;
}

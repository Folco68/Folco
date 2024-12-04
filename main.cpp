/*******************************************************************************
 *                                                                             *
 * Folco - Program allowing to quickly change the IPv4 address of an interface *
 *                     Copyright (C) 2024 Martial Demolins                     *
 *                                                                             *
 *    This program is free software: you can redistribute it and/or modify     *
 *    it under the terms of the GNU General Public License as published by     *
 *      the Free Software Foundation, either version 3 of the License, or      *
 *                      at your option) any later version                      *
 *                                                                             *
 *       This program is distributed in the hope that it will be useful        *
 *       but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
 *                 GNU General Public License for more details                 *
 *                                                                             *
 *      You should have received a copy of the GNU General Public License      *
 *     along with this program.  If not, see <https://www.gnu.org/licenses     *
 *                                                                             *
 ******************************************************************************/

#include "UI/TrayIcon.hpp"
#include <QApplication>
#include <QGuiApplication>

int main(int argc, char* argv[])
{
    // Create and set up the application
    QApplication Application(argc, argv);
    Application.setWindowIcon(QIcon(":/Icons/IconBase.ico"));

    // We don't want the application to exit when a window is closed
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

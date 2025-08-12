/**************************************************************************************** 
 *                                                                                      * 
 *     Folco - Program allowing to quickly change the IPv4 address of an interface      * 
 *                       Copyright (C) 2024-2025 Martial Demolins                       * 
 *                                                                                      * 
 *         This program is free software: you can redistribute it and/or modify         * 
 *         it under the terms of the GNU General Public License as published by         * 
 *          the Free Software Foundation, either version 3 of the License, or           * 
 *                          (at your option) any later version                          * 
 *                                                                                      * 
 *            This program is distributed in the hope that it will be useful            * 
 *            but WITHOUT ANY WARRANTY; without even the implied warranty of            * 
 *            MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             * 
 *                     GNU General Public License for more details                      * 
 *                                                                                      * 
 *          You should have received a copy of the GNU General Public License           * 
 *         along with this program.  If not, see <https://www.gnu.org/licenses>         * 
 *                                                                                      * 
 ****************************************************************************************/

#include "DlgLog.hpp"
#include "../Global.hpp"
#include "../Logger.hpp"
#include "ui_DlgLog.h"
#include <QPushButton>
#include <QScrollBar>

DlgLog* DlgLog::dlglog;

DlgLog::DlgLog()
    : QDialog(nullptr)
    , ui(new Ui::DlgLog)
    , Dialog(this)
{
    // Setup UI
    ui->setupUi(this);
    setWindowTitle(QString("%1 - Log").arg(APPLICATION_NAME)); //
    setAttribute(Qt::WA_DeleteOnClose);                        // Non modal

    // Display log and scroll to the end
    ui->TextEditLog->setPlainText(Logger::instance()->log());
    scrollToTheEnd();

    // Log has been updated
    connect(ui->ButtonClose, &QPushButton::clicked, this, [this]() { close(); });
    connect(Logger::instance(), &Logger::newLogEntry, this, [this](QString entry) {
        ui->TextEditLog->appendPlainText(entry);
        scrollToTheEnd();
    });
}

DlgLog::~DlgLog()
{
    delete ui;
    dlglog = nullptr;
}

void DlgLog::showDlgLog()
{
    if (dlglog == nullptr) {
        dlglog = new DlgLog;
        dlglog->show();
    }
    else {
        dlglog->show();
        dlglog->setVisible(true);
        dlglog->activateWindow();
    }
}

void DlgLog::scrollToTheEnd()
{
    ui->TextEditLog->verticalScrollBar()->setValue(ui->TextEditLog->verticalScrollBar()->maximum());
};

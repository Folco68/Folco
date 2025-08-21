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

#include "DlgHelp.hpp"
#include "../BeforeRelease.hpp"
#include "../Global.hpp"
#include "ui_DlgHelp.h"
#include <QFile>
#include <QPushButton>
#include <QTextStream>

DlgHelp::DlgHelp()
    : QDialog(nullptr)
    , Dialog(this)
    , ui(new Ui::DlgHelp)
{
    // Setup UI
    ui->setupUi(this);
    setWindowTitle(QString("%1 - Informations").arg(APPLICATION_NAME));

    // Readme
    QFile FileReadme(":/Docs/README");
    if (FileReadme.open(QIODeviceBase::ReadOnly)) {
        QTextStream StreamReadme(&FileReadme);
        QString     Readme = StreamReadme.readAll();
        Readme.replace("PLACEHOLDER_POSITION_STR", POSITION_STR);
        Readme.replace("PLACEHOLDER_COPYRIGHT_STR", COPYRIGHT_STR);
        Readme.replace("PLACEHOLDER_APPLICATION_VERSION_STR", APPLICATION_VERSION_STR);
        Readme.replace("PLACEHOLDER_QT_VERSION_STR", QT_VERSION_STR);
        Readme.replace("PLACEHOLDER_EMAIL_PERSONAL", EMAIL_PERSONAL);
        Readme.replace("PLACEHOLDER_EMAIL_PROFESSIONAL", EMAIL_PROFESSIONAL);
        ui->TextEditReadme->setPlainText(Readme);
    }

    // Changelog
    QFile FileChangelog(":/Docs/Changelog");
    if (FileChangelog.open(QIODeviceBase::ReadOnly)) {
        QTextStream StreamChangelog(&FileChangelog);
        ui->TextEditChangelog->setPlainText(StreamChangelog.readAll());
    }

    // License
    QFile FileLicense(":/Docs/LICENSE");
    if (FileLicense.open(QIODeviceBase::ReadOnly)) {
        QTextStream StreamLicense(&FileLicense);
        ui->TextEditLicense->setPlainText(StreamLicense.readAll());
    }

    // Always display the first index
    ui->Tabs->setCurrentIndex(0);

    // Connections
    connect(ui->ButtonClose, &QPushButton::clicked, this, [this]() { accept(); });
}

DlgHelp::~DlgHelp()
{
    delete ui;
}

void DlgHelp::execDlgHelp()
{
    DlgHelp* Dlg = new DlgHelp; // Deleted on close
    Dlg->exec();
    delete Dlg;
}

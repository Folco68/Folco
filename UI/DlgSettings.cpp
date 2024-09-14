#include "DlgSettings.hpp"
#include "../Global.hpp"
#include "../Settings.hpp"
#include "ui_DlgSettings.h"

DlgSettings::DlgSettings()
    : ui(new Ui::DlgSettings)
{
    // Setup window
    ui->setupUi(this);
    setWindowTitle(QString("%1 - Settings").arg(APPLICATION_NAME));
    adjustSize();

    // Set the UI according to the settings
    ui->CheckShowOnlyEthernetWiFi->setChecked(Settings::instance()->showOnlyEthernetWifi());
    ui->CheckShowOnlyUp->setChecked(Settings::instance()->showOnlyUp());
    ui->CheckShowOnlyPredefined->setChecked(Settings::instance()->showOnlyPredefined());

    // Connections
    connect(ui->ButtonOK, &QPushButton::clicked, this, [this]() { accept(); });
    connect(ui->ButtonCancel, &QPushButton::clicked, this, [this]() { reject(); });
}

DlgSettings::~DlgSettings()
{
    delete ui;
}

void DlgSettings::execDlgSettings()
{

    DlgSettings* Dlg = new DlgSettings();

    if (Dlg->exec() == QDialog::Accepted) {
        Settings::instance()->setShowOnlyEthernetWifi(Dlg->ui->CheckShowOnlyEthernetWiFi->isChecked());
        Settings::instance()->setShowOnlyUp(Dlg->ui->CheckShowOnlyUp->isChecked());
        Settings::instance()->setShowOnlyPredefined(Dlg->ui->CheckShowOnlyPredefined->isChecked());
    }

    delete Dlg;
}

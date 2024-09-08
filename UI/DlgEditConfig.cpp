#include "DlgEditConfig.hpp"
#include "../Global.hpp"
#include "ui_DlgEditConfig.h"
#include <QPushButton>

DlgEditConfig::DlgEditConfig(QWidget* parent, Configuration Config)
    : QDialog(parent)
    , ui(new Ui::DlgEditConfig)
{
    ui->setupUi(this);

    // According to the Configuration content, set the window title and the edition fields
    if (Config.isValid()) {
        setWindowTitle(QString("%1 - Edit configuration").arg(APPLICATION_NAME));
        ui->EditIPaddress->setText(Config.ipAddress());
        ui->EditNetworkMask->setText(Config.networkMask());
        ui->EditGateway->setText(Config.gateway());
    }
    else {
        setWindowTitle(QString("%1 - New configuration").arg(APPLICATION_NAME));
    }

    // Adjust the dialog size
    adjustSize();

    // Buttons connections
    connect(ui->ButtonCancel, &QPushButton::clicked, this, [this]() { reject(); });
    connect(ui->ButtonOK, &QPushButton::clicked, this, [this]() { accept(); });
}

DlgEditConfig::~DlgEditConfig()
{
    delete ui;
}

Configuration DlgEditConfig::dlgNewConfig(QWidget* parent)
{
    Configuration  Config;
    DlgEditConfig* Dlg = new DlgEditConfig(parent);
    if (Dlg->exec() == QDialog::Accepted) {
        Config.setConfig(Dlg->ui->EditIPaddress->text(), Dlg->ui->EditNetworkMask->text(), Dlg->ui->EditGateway->text());
    }
    return Config;
}

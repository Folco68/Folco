#include "DlgEditConfig.hpp"
#include "../Global.hpp"
#include "ui_DlgEditConfig.h"
#include <QPushButton>

DlgEditConfig::DlgEditConfig(QWidget* parent, PredefinedIP ip)
    : QDialog(parent)
    , ui(new Ui::DlgEditConfig)
{
    ui->setupUi(this);

    // According to the Configuration content, set the window title and the edition fields
    if (ip.isValid()) {
        setWindowTitle(QString("%1 - Edit configuration").arg(APPLICATION_NAME));
        ui->EditIPaddress->setText(ip.ipAddress());
        ui->EditNetworkMask->setText(ip.networkMask());
        ui->EditGateway->setText(ip.gateway());
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

PredefinedIP DlgEditConfig::dlgNewConfig(QWidget* parent)
{
    PredefinedIP   IP;
    DlgEditConfig* Dlg = new DlgEditConfig(parent);
    if (Dlg->exec() == QDialog::Accepted) {
        IP.setPredefinedIP(Dlg->ui->EditIPaddress->text(), Dlg->ui->EditNetworkMask->text(), Dlg->ui->EditGateway->text());
    }
    return IP;
}

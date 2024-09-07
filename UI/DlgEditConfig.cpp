#include "DlgEditConfig.hpp"
#include "../Global.hpp"
#include "ui_DlgEditConfig.h"
#include <QPushButton>

DlgEditConfig::DlgEditConfig(QWidget* parent, QList<QString> Configuration)
    : QDialog(parent)
    , ui(new Ui::DlgEditConfig)
{
    ui->setupUi(this);

    // According to the Configuration content, set the window title and the edition fields
    if (Configuration.isEmpty()) {
        setWindowTitle(QString("%1 - New configuration").arg(APPLICATION_NAME));
    }
    else {
        setWindowTitle(QString("%1 - Edit configuration").arg(APPLICATION_NAME));
        ui->EditIPaddress->setText(Configuration.at(0));
        ui->EditNetworkMask->setText(Configuration.at(1));
        ui->EditGateway->setText(Configuration.at(2));
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

QList<QString> DlgEditConfig::dlgNewConfig(QWidget* parent)
{
    QList<QString> Configuration;
    DlgEditConfig* Dlg = new DlgEditConfig(parent);
    if (Dlg->exec() == QDialog::Accepted) {
        Configuration << Dlg->ui->EditIPaddress->text() << Dlg->ui->EditNetworkMask->text() << Dlg->ui->EditGateway->text();
    }
    return Configuration;
}

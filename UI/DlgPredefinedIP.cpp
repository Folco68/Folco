#include "DlgPredefinedIP.hpp"
#include "ui_DlgPredefinedIP.h"

// Support null ptrs. Useless here, but it supports it anyway
DlgPredefinedIP::DlgPredefinedIP(QWidget* parent, QString* name, QString* ip, QString* networkmask, QString* gateway)
    : QDialog(parent)
    , ui(new Ui::DlgPredefinedIP)
{
    ui->setupUi(this);

    // Fill the UI if arguments were provided
    if (name != nullptr) {
        ui->EditName->setText(*name);
    }

    if (ip != nullptr) {
        ui->EditIPaddress->setText(*ip);
    }

    if (networkmask != nullptr) {
        ui->EditNetworkMask->setText(*networkmask);
    }

    if (gateway != nullptr) {
        ui->EditGateway->setText(*gateway);
    }

    connect(ui->ButtonOK, &QPushButton::clicked, this, [this]() { accept(); });
    connect(ui->ButtonCancel, &QPushButton::clicked, this, [this]() { reject(); });
    connect(ui->EditIPaddress, &QLineEdit::textChanged, this, [this]() { ui->ButtonOK->setEnabled(ui->EditIPaddress->hasAcceptableInput()); });
}

DlgPredefinedIP::~DlgPredefinedIP()
{
    delete ui;
}

int DlgPredefinedIP::newPredefinedIP(QWidget* parent, QString* name, QString* ip, QString* networkmask, QString* gateway)
{
    int RetVal;

    DlgPredefinedIP* Dlg = new DlgPredefinedIP(parent);
    Dlg->setWindowTitle("New predefined IP");
    RetVal = Dlg->exec();

    if (RetVal == QDialog::Accepted) {
        *name        = Dlg->ui->EditName->text();
        *ip          = Dlg->ui->EditIPaddress->text();
        *networkmask = Dlg->ui->EditNetworkMask->text();
        *gateway     = Dlg->ui->EditGateway->text();
    }

    delete Dlg;
    return RetVal;
}

int DlgPredefinedIP::editPredefinedIP(QWidget* parent, QString* name, QString* ip, QString* networkmask, QString* gateway)
{
    int RetVal;

    DlgPredefinedIP* Dlg = new DlgPredefinedIP(parent, name, ip, networkmask, gateway);
    Dlg->setWindowTitle("Edit predefined IP");
    RetVal = Dlg->exec();

    if (RetVal == QDialog::Accepted) {
        *name        = Dlg->ui->EditName->text();
        *ip          = Dlg->ui->EditIPaddress->text();
        *networkmask = Dlg->ui->EditNetworkMask->text();
        *gateway     = Dlg->ui->EditGateway->text();
    }

    delete Dlg;
    return RetVal;
}
#include "DlgEditPredefinedIP.hpp"
#include "ui_DlgEditPredefinedIP.h"

// Support null ptrs. Useless here, but it supports it anyway
DlgEditPredefinedIP::DlgEditPredefinedIP(QWidget* parent, QString* name, QString* ip, QString* networkmask, QString* gateway)
    : QDialog(parent)
    , ui(new Ui::DlgEditPredefinedIP)
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

DlgEditPredefinedIP::~DlgEditPredefinedIP()
{
    delete ui;
}

int DlgEditPredefinedIP::newPredefinedIP(QWidget* parent, QString* name, QString* ip, QString* networkmask, QString* gateway)
{
    int RetVal;

    DlgEditPredefinedIP* Dlg = new DlgEditPredefinedIP(parent);
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

int DlgEditPredefinedIP::editPredefinedIP(QWidget* parent, QString* name, QString* ip, QString* networkmask, QString* gateway)
{
    int RetVal;

    DlgEditPredefinedIP* Dlg = new DlgEditPredefinedIP(parent, name, ip, networkmask, gateway);
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

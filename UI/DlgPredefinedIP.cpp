#include "DlgPredefinedIP.hpp"
#include "ui_DlgPredefinedIP.h"
#include <QPalette>
#include <QStringList>

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
        ui->EditIP->setText(*ip);
    }

    if (networkmask != nullptr) {
        ui->EditMask->setText(*networkmask);
    }

    if (gateway != nullptr) {
        ui->EditGateway->setText(*gateway);
    }

    // Connections
    connect(ui->ButtonOK, &QPushButton::clicked, this, [this]() { accept(); });
    connect(ui->ButtonCancel, &QPushButton::clicked, this, [this]() { reject(); });
    connect(ui->EditIP, &QLineEdit::textChanged, this, [this]() { updateButtonOK(); });
    connect(ui->EditMask, &QLineEdit::textChanged, this, [this]() { updateButtonOK(); });
    connect(ui->EditGateway, &QLineEdit::textChanged, this, [this]() { updateButtonOK(); });

    // Set OK button according to IPs
    updateButtonOK();
}

DlgPredefinedIP::~DlgPredefinedIP()
{
    delete ui;
}

void DlgPredefinedIP::updateButtonOK()
{
    ui->ButtonOK->setEnabled((ui->EditIP->hasAcceptableInput() && !ui->EditIP->text().isEmpty()) && ui->EditMask->hasAcceptableInput() && ui->EditGateway->hasAcceptableInput());
}

int DlgPredefinedIP::newPredefinedIP(QWidget* parent, QString* name, QString* ip, QString* networkmask, QString* gateway)
{
    int RetVal;

    DlgPredefinedIP* Dlg = new DlgPredefinedIP(parent);
    Dlg->setWindowTitle("New predefined IP");
    RetVal = Dlg->exec();

    if (RetVal == QDialog::Accepted) {
        *name        = Dlg->ui->EditName->text();
        *ip          = Dlg->ui->EditIP->text();
        *networkmask = Dlg->ui->EditMask->text();
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
        *ip          = Dlg->ui->EditIP->text();
        *networkmask = Dlg->ui->EditMask->text();
        *gateway     = Dlg->ui->EditGateway->text();
    }

    delete Dlg;
    return RetVal;
}

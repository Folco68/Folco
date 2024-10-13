#include "DlgPredefinedIP.hpp"
#include "ui_DlgPredefinedIP.h"
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
        QStringList Bytes = ip->split('.');
        ui->SpinBox11->setValue(Bytes.at(0).toInt());
        ui->SpinBox12->setValue(Bytes.at(1).toInt());
        ui->SpinBox13->setValue(Bytes.at(2).toInt());
        ui->SpinBox14->setValue(Bytes.at(3).toInt());
    }

    if (networkmask != nullptr) {
        QStringList Bytes = networkmask->split('.');
        ui->SpinBox21->setValue(Bytes.at(0).toInt());
        ui->SpinBox22->setValue(Bytes.at(1).toInt());
        ui->SpinBox23->setValue(Bytes.at(2).toInt());
        ui->SpinBox24->setValue(Bytes.at(3).toInt());
    }

    if (gateway != nullptr) {
        QStringList Bytes = gateway->split('.');
        ui->SpinBox31->setValue(Bytes.at(0).toInt());
        ui->SpinBox32->setValue(Bytes.at(1).toInt());
        ui->SpinBox33->setValue(Bytes.at(2).toInt());
        ui->SpinBox34->setValue(Bytes.at(3).toInt());
    }

    // Allow the '.' key to act like the tab key when typing an ip
    // Also used when pasting an IP
    ui->SpinBox11->setNextByteBox(ui->SpinBox12);
    ui->SpinBox12->setNextByteBox(ui->SpinBox13);
    ui->SpinBox13->setNextByteBox(ui->SpinBox14);

    ui->SpinBox21->setNextByteBox(ui->SpinBox22);
    ui->SpinBox22->setNextByteBox(ui->SpinBox23);
    ui->SpinBox23->setNextByteBox(ui->SpinBox24);

    ui->SpinBox31->setNextByteBox(ui->SpinBox32);
    ui->SpinBox32->setNextByteBox(ui->SpinBox33);
    ui->SpinBox33->setNextByteBox(ui->SpinBox34);

    // Set the previous SpinBox for the 3 lower bytes
    // Used when pasting an IP
    ui->SpinBox12->setPreviousByteBox(ui->SpinBox11);
    ui->SpinBox13->setPreviousByteBox(ui->SpinBox12);
    ui->SpinBox14->setPreviousByteBox(ui->SpinBox13);

    ui->SpinBox22->setPreviousByteBox(ui->SpinBox21);
    ui->SpinBox23->setPreviousByteBox(ui->SpinBox22);
    ui->SpinBox24->setPreviousByteBox(ui->SpinBox23);

    ui->SpinBox32->setPreviousByteBox(ui->SpinBox31);
    ui->SpinBox33->setPreviousByteBox(ui->SpinBox32);
    ui->SpinBox34->setPreviousByteBox(ui->SpinBox33);


    // Dialog buttons
    connect(ui->ButtonOK, &QPushButton::clicked, this, [this]() { accept(); });
    connect(ui->ButtonCancel, &QPushButton::clicked, this, [this]() { reject(); });
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
        *ip          = QString("%1.%2.%3.%4").arg(Dlg->ui->SpinBox11->value()).arg(Dlg->ui->SpinBox12->value()).arg(Dlg->ui->SpinBox13->value()).arg(Dlg->ui->SpinBox14->value());
        *networkmask = QString("%1.%2.%3.%4").arg(Dlg->ui->SpinBox21->value()).arg(Dlg->ui->SpinBox22->value()).arg(Dlg->ui->SpinBox23->value()).arg(Dlg->ui->SpinBox24->value());
        *gateway     = QString("%1.%2.%3.%4").arg(Dlg->ui->SpinBox31->value()).arg(Dlg->ui->SpinBox32->value()).arg(Dlg->ui->SpinBox33->value()).arg(Dlg->ui->SpinBox34->value());
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
        *ip          = QString("%1.%2.%3.%4").arg(Dlg->ui->SpinBox11->value()).arg(Dlg->ui->SpinBox12->value()).arg(Dlg->ui->SpinBox13->value()).arg(Dlg->ui->SpinBox14->value());
        *networkmask = QString("%1.%2.%3.%4").arg(Dlg->ui->SpinBox21->value()).arg(Dlg->ui->SpinBox22->value()).arg(Dlg->ui->SpinBox23->value()).arg(Dlg->ui->SpinBox24->value());
        *gateway     = QString("%1.%2.%3.%4").arg(Dlg->ui->SpinBox31->value()).arg(Dlg->ui->SpinBox32->value()).arg(Dlg->ui->SpinBox33->value()).arg(Dlg->ui->SpinBox34->value());
    }

    delete Dlg;
    return RetVal;
}

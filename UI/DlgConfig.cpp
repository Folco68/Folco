#include "DlgConfig.hpp"
#include "ui_DlgConfig.h"
#include <QHostAddress>
#include <QList>
#include <QNetworkInterface>
#include <QString>
#include <QTableWidget>
#include <QTableWidgetItem>

DlgConfig::DlgConfig(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::DlgConfig)
{
    ui->setupUi(this);
    refreshInterfaces();
}

DlgConfig::~DlgConfig()
{
    delete ui;
}

int DlgConfig::execDlgConfig()
{
    DlgConfig* Dlg = new DlgConfig;
    Dlg->exec();
    delete Dlg;
    return 0;
}

void DlgConfig::refreshInterfaces()
{
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///                                                                                                                             ///
    ///                                                         Remove current data                                                 ///
    ///                                                                                                                             ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Delete the QTableWidgetItem of each cell
    int RowCount    = ui->TableInterfaces->rowCount();
    int ColumnCount = ui->TableInterfaces->columnCount();

    for (int i = 0; i < RowCount; i++) {
        for (int j = 0; j < ColumnCount; j++) {
            delete ui->TableInterfaces->item(i, j);
        }
    }

    // All cells are empty, remove them
    ui->TableInterfaces->setRowCount(0);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///                                                                                                                             ///
    ///                                       List interfaces and display their data in the table                                   ///
    ///                                                                                                                             ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    QList<QNetworkInterface> Interfaces = QNetworkInterface::allInterfaces();

    for (int i = 0; i < Interfaces.size(); i++) {
        // Current interface
        QNetworkInterface Interface = Interfaces.at(i);

        // Interface properties
        QString                          Name            = Interface.name();
        QString                          LongName        = Interface.humanReadableName();
        QNetworkInterface::InterfaceType Type            = Interface.type();
        QHostAddress                     IPaddress       = Interface.addressEntries().at(0).ip();
        QHostAddress                     NetworkMask     = Interface.addressEntries().at(0).netmask();
        QString                          HardwareAddress = Interface.hardwareAddress();

        // Create graphical items
        QTableWidgetItem* ItemName            = new QTableWidgetItem(Name);
        QTableWidgetItem* ItemLongName        = new QTableWidgetItem(LongName);
        QTableWidgetItem* ItemType            = new QTableWidgetItem(QString::number(Type));
        QTableWidgetItem* ItemIPaddress       = new QTableWidgetItem(IPaddress.toString());
        QTableWidgetItem* ItemNetworkMask     = new QTableWidgetItem(NetworkMask.toString());
        QTableWidgetItem* ItemHardwareAddress = new QTableWidgetItem(HardwareAddress);

        // Install graphical items
        ui->TableInterfaces->setRowCount(i + 1);
        ui->TableInterfaces->setItem(i, COLUMN_NAME, ItemName);
        ui->TableInterfaces->setItem(i, COLUMN_LONG_NAME, ItemLongName);
        ui->TableInterfaces->setItem(i, COLUMN_TYPE, ItemType);
        ui->TableInterfaces->setItem(i, COLUMN_IP_ADDRESS, ItemIPaddress);
        ui->TableInterfaces->setItem(i, COLUMN_NETWORK_MASK, ItemNetworkMask);
        ui->TableInterfaces->setItem(i, COLUMN_HARDWARE_ADDRESS, ItemHardwareAddress);
    }
}

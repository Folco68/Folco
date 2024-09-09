#include "DlgSettings.hpp"
#include "../Global.hpp"
#include "ui_DlgSettings.h"

DlgSettings::DlgSettings(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::DlgSettings)
{
    // Setup window
    ui->setupUi(this);
    setWindowTitle(QString("%1 - Settings").arg(APPLICATION_NAME));

    adjustSize();
}

DlgSettings::~DlgSettings()
{
    delete ui;
}

int DlgSettings::execDlgSettings()
{
    return 0;
}

#include "DlgMergeConnections.hpp"
#include "ui_DlgMergeConnections.h"

DlgMergeConnections::DlgMergeConnections(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DlgMergeConnections)
{
    ui->setupUi(this);
}

DlgMergeConnections::~DlgMergeConnections()
{
    delete ui;
}

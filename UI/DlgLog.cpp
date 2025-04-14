#include "DlgLog.hpp"
#include <QPushButton>
#include <QScrollBar>
#include "../Global.hpp"
#include "../Logger.hpp"
#include "ui_DlgLog.h"

DlgLog *DlgLog::dlglog;

DlgLog::DlgLog()
    : QDialog(nullptr)
    , ui(new Ui::DlgLog)
{
    // Setup UI
    ui->setupUi(this);
    setWindowTitle(QString("%1 - Log").arg(APPLICATION_NAME)); //
    setAttribute(Qt::WA_DeleteOnClose);                        // Non modal

    // Display log and scroll to the end
    ui->TextEditLog->setPlainText(Logger::instance()->log());
    scrollToTheEnd();

    // Log has been updated
    connect(ui->ButtonClose, &QPushButton::clicked, this, [this]() { close(); });
    connect(Logger::instance(), &Logger::newLogEntry, this, [this](QString entry) {
        ui->TextEditLog->appendPlainText(entry);
        scrollToTheEnd();
    });
}

DlgLog::~DlgLog()
{
    delete ui;
    dlglog = nullptr;
}

void DlgLog::showDlgLog()
{
    if (dlglog == nullptr) {
        dlglog = new DlgLog;
        dlglog->show();
    } else {
        dlglog->show();
        dlglog->setVisible(true);
        dlglog->activateWindow();
    }
}

void DlgLog::scrollToTheEnd()
{
    ui->TextEditLog->verticalScrollBar()->setValue(ui->TextEditLog->verticalScrollBar()->maximum());
};

#include "WindowLog.hpp"
#include <QScrollBar>
#include "../Logger.hpp"
#include "ui_WindowLog.h"

WindowLog *WindowLog::windowlog;

WindowLog::WindowLog()
    : QWidget(nullptr, Qt::Window)
    , ui(new Ui::WindowLog)
{
    // Non-modal window, delete it on close
    setAttribute(Qt::WA_DeleteOnClose);

    ui->setupUi(this);

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

WindowLog::~WindowLog()
{
    delete ui;
    windowlog = nullptr;
}

void WindowLog::showWindowLog()
{
    if (windowlog == nullptr) {
        windowlog = new WindowLog;
        windowlog->show();
    } else {
        windowlog->show();
        windowlog->setVisible(true);
        windowlog->activateWindow();
    }
}

void WindowLog::scrollToTheEnd()
{
    ui->TextEditLog->verticalScrollBar()->setValue(ui->TextEditLog->verticalScrollBar()->maximum());
};

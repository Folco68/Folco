#include "IPlineEdit.hpp"
#include <QApplication>
#include <QClipboard>
#include <QKeySequence>
#include <QMessageBox>
#include <QMimeData>
#include <QShortcut>

IPlineEdit::IPlineEdit(QWidget* parent)
    : QLineEdit(parent)
{
    this->validator = new IPvalidator;
    setValidator(this->validator);
}

IPlineEdit::~IPlineEdit()
{
    setValidator(nullptr);
    delete this->validator;
}

void IPlineEdit::dragEnterEvent(QDragEnterEvent* event)
{
    if (event->mimeData()->hasFormat("text/plain")) {
        QString IP(event->mimeData()->text());
        if (isIPvalid(IP)) {
            event->acceptProposedAction();
        }
    }
}

void IPlineEdit::dropEvent(QDropEvent* event)
{
    setText(event->mimeData()->text());
}

/*
void IPlineEdit::paste()
{
    const QClipboard* Clipboard = QApplication::clipboard();
    if (Clipboard->mimeData()->hasFormat("text/plain")) {
        QString IP(Clipboard->mimeData()->text());
        if (isIPvalid(IP)) {
            setText(IP);
        }
    }
}
*/

bool IPlineEdit::isIPvalid(QString ip) const
{
    // Need 4 bytes dot separated for IPv4 address
    QStringList Bytes(ip.split("."));
    if (Bytes.count() != 4) {
        return false;
    }

    // Need 0 < int < 255 for each byte
    for (int i = 0; i < Bytes.count(); i++) {
        bool IsInt;
        int  Byte = Bytes.at(i).toInt(&IsInt);
        if ((!IsInt) || (Byte < 0) || (Byte > 255)) {
            return false;
        }
    }

    // The IP is valid
    return true;
}

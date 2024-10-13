#include "IPspinBox.hpp"
#include <QAbstractSpinBox>
#include <QMimeData>

IPspinBox::IPspinBox(QWidget* parent)
    : QSpinBox(parent)
    , NextByteBox(nullptr)
    , PreviousByteBox(nullptr)
{
    this->setAcceptDrops(true);
}

// focusInEvent is protected, impossible to just connect it to selectAll(), need to redefine it
// Used because of the limitation to 255 in each box. It's painful to remove chars before entering new ones
void IPspinBox::focusInEvent(QFocusEvent* event)
{
    QAbstractSpinBox::focusInEvent(event);
    selectAll();
}

void IPspinBox::mouseReleaseEvent(QMouseEvent* event)
{
    QAbstractSpinBox::mouseReleaseEvent(event);
    selectAll();
}

void IPspinBox::keyReleaseEvent(QKeyEvent* event)
{
    if ((event->text() == QString(".")) && (this->NextByteBox != nullptr)) {
        this->NextByteBox->setFocus();
    }
}

void IPspinBox::setNextByteBox(IPspinBox* box)
{
    this->NextByteBox = box;
}

void IPspinBox::setPreviousByteBox(IPspinBox* box)
{
    this->PreviousByteBox = box;
}

void IPspinBox::dragEnterEvent(QDragEnterEvent* event)
{
    if (event->mimeData()->hasFormat("text/plain")) {
        // Validate IP
        QString     IP(event->mimeData()->text());
        QStringList Bytes(IP.split("."));

        // Need 4 bytes for IPv4 address
        if (Bytes.count() == 4) {
            // Need int 0 < int < 255 for each byte
            for (int i = 0; i < Bytes.count(); i++) {
                bool IsInt;
                int  Byte = Bytes.at(i).toInt(&IsInt);

                // 0 < int < 255 ?
                if ((!IsInt) && ((Byte < 0) || (Byte > 255))) {
                    return;
                }
            }
        }

        // 4 times 0 < int < 255 => valid
        event->acceptProposedAction();
    }
}

void IPspinBox::dropEvent(QDropEvent* event)
{
}

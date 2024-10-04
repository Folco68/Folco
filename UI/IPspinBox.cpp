#include "IPspinBox.hpp"
#include <QAbstractSpinBox>

IPspinBox::IPspinBox(QWidget* parent)
    : QSpinBox(parent)
    , NextByteBox(nullptr)
{
}

// focusInEvent is protected, impossible to just connect it to selectAll(), need to redefine it
// Used because of the limitation to 255 in each box. It's painfull to remove chars before entering new ones
void IPspinBox::focusInEvent(QFocusEvent* event)
{
    QAbstractSpinBox::focusInEvent(event);
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

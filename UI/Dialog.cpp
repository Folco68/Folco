#include "Dialog.hpp"

QDialog* Dialog::dialog = nullptr;

Dialog::Dialog(QDialog* dlg)
{
    dialog = dlg;
}

Dialog::~Dialog()
{
    dialog = nullptr;
}

bool Dialog::display()
{
    if (dialog != nullptr) {
        dialog->activateWindow();
    }
    return dialog != nullptr;
}

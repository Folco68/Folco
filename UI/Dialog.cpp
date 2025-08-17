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
    // If there is one opened dialog, bring it to the foreground
    if (dialog != nullptr) {
        dialog->activateWindow();
    }

    // Return true if a dialog was displayed, to prevent the tray menu from popping
    return dialog != nullptr;
}

#ifndef DIALOG_HPP
#define DIALOG_HPP

#include <QDialog>

/********************************************************************************************************************** 
 *                                                                                                                    * 
 *                                                       Dialog                                                       * 
 *                                                                                                                    * 
 *                                          All dialogs inherit this class.                                           * 
 *                     Its purpose is to prevent from opening multiple dialogs at the same time.                      * 
 *      To achieve this, this class holds a static pointer to the opened QDialog, or nullptr if none is opened.       * 
 *           When the icon in the system tray is clicked, bring the dialog to the foreground if one exists,           * 
 *                          preventing from opening the context menu and creating a new one.                          * 
 *                                                                                                                    * 
 **********************************************************************************************************************/

class Dialog
{
  public:
    static bool display(); // Bring a dialog to the foreground if one exists. Return true if one exists

  protected:              // No stand-alone object. Must be built within an inheritance
    Dialog(QDialog* dlg); //
    ~Dialog();            //

  private:
    static QDialog* dialog; // Pointer to the existing dialog, or nullptr if non exists
};

#endif // DIALOG_HPP

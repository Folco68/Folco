#ifndef DIALOG_HPP
#define DIALOG_HPP

#include <QDialog>

/********************************************************************************************************************** 
 *                                                                                                                    * 
 *                                                       Dialog                                                       * 
 *                                                                                                                    * 
 *                                          All dialogs inherit this class.                                           * 
 *                     Its purpose is to prevent from opening multiples dialogs at the same time.                     * 
 *                          To achieve this, this class holds a static pointer to a QDialog,                          * 
 *                           and brings the current dialog to the foreground if one exists.                           * 
 *                                                                                                                    * 
 **********************************************************************************************************************/

class Dialog
{
  public:
    static bool display();

  protected:
    Dialog(QDialog* dlg);
    ~Dialog();

  private:
    static QDialog* dialog;
};

#endif // DIALOG_HPP

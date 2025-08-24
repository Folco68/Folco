/**************************************************************************************** 
 *                                                                                      * 
 *     Folco - Program allowing to quickly change the IPv4 address of an interface      * 
 *                       Copyright (C) 2024-2025 Martial Demolins                       * 
 *                                                                                      * 
 *         This program is free software: you can redistribute it and/or modify         * 
 *         it under the terms of the GNU General Public License as published by         * 
 *          the Free Software Foundation, either version 3 of the License, or           * 
 *                          (at your option) any later version                          * 
 *                                                                                      * 
 *            This program is distributed in the hope that it will be useful            * 
 *            but WITHOUT ANY WARRANTY; without even the implied warranty of            * 
 *            MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             * 
 *                     GNU General Public License for more details                      * 
 *                                                                                      * 
 *          You should have received a copy of the GNU General Public License           * 
 *         along with this program.  If not, see <https://www.gnu.org/licenses>         * 
 *                                                                                      * 
 ****************************************************************************************/

#ifndef DLG_PREDEFINED_IP_HPP
#define DLG_PREDEFINED_IP_HPP

#include "Dialog.hpp"
#include <QDialog>

namespace Ui {
class DlgPredefinedIP;
}

/********************************************************************************************************************** 
 *                                                                                                                    * 
 *                                                  DlgPredefinedIP                                                   * 
 *                                                                                                                    * 
 *                                    This class allows to edit a pre-defined IP.                                     * 
 *                                                                                                                    * 
 **********************************************************************************************************************/

class DlgPredefinedIP
    : public QDialog
    , public Dialog
{
    Q_OBJECT

  public:
    static int newPredefinedIP(QString* name, QString* ip, QString* networkmask, QString* gateway);
    static int editPredefinedIP(QString* name, QString* ip, QString* networkmask, QString* gateway);

  private:
    explicit DlgPredefinedIP(QString title = nullptr); // Delegate constructor
    DlgPredefinedIP(QString* name, QString* ip, QString* networkmask, QString* gateway);
    ~DlgPredefinedIP();
    void updateButtonOK();

    Ui::DlgPredefinedIP* ui;
};

#endif // DLG_PREDEFINED_IP_HPP

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

#ifndef DLGMERGECONFIGURATIONS_HPP
#define DLGMERGECONFIGURATIONS_HPP

#include "Dialog.hpp"
#include <QDialog>

namespace Ui {
class DlgMergeConfigurations;
}

/********************************************************************************************************************** 
 *                                                                                                                    * 
 *                                               DlgMergeConfigurations                                               * 
 *                                                                                                                    * 
 * This class allows to merge Configurations, in other words to give the configuration of an interface to another one * 
 *                                                                                                                    * 
 **********************************************************************************************************************/

class DlgMergeConfigurations
    : public QDialog
    , public Dialog
{
    Q_OBJECT

  public:
    explicit DlgMergeConfigurations(QWidget* parent = nullptr);
    ~DlgMergeConfigurations();

  private:
    Ui::DlgMergeConfigurations* ui;
};

#endif // DLGMERGECONFIGURATIONS_HPP

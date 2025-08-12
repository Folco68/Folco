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

#ifndef IPLINEEDIT_HPP
#define IPLINEEDIT_HPP

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QLineEdit>
#include <QString>
#include <QWidget>

/********************************************************************************************************************** 
 *                                                                                                                    * 
 *                                                     IPlineEdit                                                     * 
 *                                                                                                                    * 
 *                          This class extends QLineEdit by checking the validity of an IP.                           * 
 *                                                                                                                    * 
 **********************************************************************************************************************/

class IPlineEdit: public QLineEdit
{
  public:
    IPlineEdit(QWidget* parent);
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dropEvent(QDropEvent* event) override;

  private:
    bool isIPvalid(QString ip) const;
};

#endif // IPLINEEDIT_HPP

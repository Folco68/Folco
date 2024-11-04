/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Folco - Program allowing to quickly change an IPv4 on an interface      //
// Copyright (C) 2024 Martial Demolins                                     //
//                                                                         //
// This program is free software: you can redistribute it and/or modify    //
// it under the terms of the GNU General Public License as published by    //
// the Free Software Foundation, either version 3 of the License, or       //
// (at your option) any later version.                                     //
//                                                                         //
// This program is distributed in the hope that it will be useful,         //
// but WITHOUT ANY WARRANTY; without even the implied warranty of          //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           //
// GNU General Public License for more details.                            //
//                                                                         //
// You should have received a copy of the GNU General Public License       //
// along with this program.  If not, see <https://www.gnu.org/licenses/>.  //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

#ifndef DLG_INTERFACE_HPP
#define DLG_INTERFACE_HPP

#include "../Network/Interface.hpp"
#include <QDialog>
#include <QNetworkInterface>

namespace Ui {
    class DlgInterface;
}

// This dialog uses the list of PredefinedIP of an Interface to fill its table.
// Only the execDlgInterface really create and delete PredefinedIP
// All other functions just modify the tables
class DlgInterface: public QDialog
{
    Q_OBJECT

  public:
    static void execDlgInterface(QNetworkInterface NetworkInterface);
    ~DlgInterface();

  private:
    explicit DlgInterface(QNetworkInterface NetworkInterface);
    void tableSelectionChanged();
    void newPredefinedIP();
    void editPredefinedIP();
    void deletePredefinedIP();
    void moveUp();
    void moveDown();

    void writeContent(Interface* interface);

    Ui::DlgInterface* ui;
};

typedef enum {
    COLUMN_NAME,
    COLUMN_IP_ADDRESS,
    COLUMN_NETWORK_MASK,
    COLUMN_GATEWAY
} COLUMN_HEADER;

#endif // DLG_INTERFACE_HPP

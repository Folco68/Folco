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

#ifndef DLGABOUT_HPP
#define DLGABOUT_HPP

#include <QDialog>

namespace Ui {
    class DlgAbout;
}

class DlgAbout: public QDialog
{
    Q_OBJECT

  public:
    explicit DlgAbout(QWidget* parent = nullptr);
    ~DlgAbout();

  private:
    Ui::DlgAbout* ui;
};

#endif // DLGABOUT_HPP
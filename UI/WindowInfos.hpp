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

#ifndef WINDOW_INFOS_HPP
#define WINDOW_INFOS_HPP

#include <QWidget>

namespace Ui {
    class WindowInfos;
}

class WindowInfos: public QWidget
{
    Q_OBJECT

  public:
    static void showWindowInfos();

  private:
    Ui::WindowInfos*    ui;
    static WindowInfos* windowinfos;
    WindowInfos();
    ~WindowInfos();
};

#endif // WINDOW_INFOS_HPP

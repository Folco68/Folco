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

#include "IPlineEdit.hpp"
#include "IPvalidator.hpp"
#include <QApplication>
#include <QClipboard>
#include <QKeySequence>
#include <QMessageBox>
#include <QMimeData>
#include <QShortcut>

IPlineEdit::IPlineEdit(QWidget* parent)
    : QLineEdit(parent)
{
    setValidator(new IPvalidator(this));
}

void IPlineEdit::dragEnterEvent(QDragEnterEvent* event)
{
    if (event->mimeData()->hasFormat("text/plain")) {
        QString IP(event->mimeData()->text());
        if (isIPvalid(IP)) {
            event->acceptProposedAction();
        }
    }
}

void IPlineEdit::dropEvent(QDropEvent* event)
{
    setText(event->mimeData()->text());
}

bool IPlineEdit::isIPvalid(QString ip) const
{
    // Need 4 bytes dot separated for IPv4 address
    QStringList Bytes(ip.split("."));
    if (Bytes.count() != 4) {
        return false;
    }

    // Need 0 < int < 255 for each byte
    for (int i = 0; i < Bytes.count(); i++) {
        bool IsInt;
        int  Byte = Bytes.at(i).toInt(&IsInt);
        if ((!IsInt) || (Byte < 0) || (Byte > 255)) {
            return false;
        }
    }

    // The IP is valid
    return true;
}

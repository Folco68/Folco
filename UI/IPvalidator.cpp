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

#include "IPvalidator.hpp"
#include <QList>
#include <QString>

IPvalidator::IPvalidator(QObject* parent)
    : QValidator(parent)
{
}

// Allow empty string, because mask and gateway are not mandatory in a PredefinedIP
QValidator::State IPvalidator::validate(QString& input, int&) const
{
    // Counter of valid bytes, which means a non null string with 0 <= byte <= 255
    int ValidBytes = 0;

    // Accept empty string
    if (input.isEmpty()) {
        return QValidator::Acceptable;
    }

    // Need maximum 4 bytes dot separated for IPv4 address
    QList<QString> Bytes(input.split("."));
    if (Bytes.count() > 4) {
        return QValidator::Invalid;
    }

    // Need 0 < int < 255 for each byte, else it is invalid
    for (int i = 0; i < Bytes.count(); i++) {
        if (!Bytes.at(i).isEmpty()) {
            bool IsInt;
            int  Byte = Bytes.at(i).toInt(&IsInt);
            if ((!IsInt) || (Byte < 0) || (Byte > 255)) {
                return QValidator::Invalid;
            }
            ValidBytes++;
        }
    }

    // All bytes are valid or null string.
    // 4 bytes means that none of them is a null string => the result is acceptable
    return ValidBytes == 4 ? QValidator::Acceptable : QValidator::Intermediate;
}

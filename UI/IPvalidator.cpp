#include "IPvalidator.hpp"
#include <QList>
#include <QString>

IPvalidator::IPvalidator(QObject* parent)
    : QValidator {parent}
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
    // The result is acceptable if:
    // - there are 4 bytes
    // - none of them is a null string
    // As a consequence, if we have less than 4 bytes or if one is a null string, the result is intermediate
    return ValidBytes == 4 ? QValidator::Acceptable : QValidator::Intermediate;
}

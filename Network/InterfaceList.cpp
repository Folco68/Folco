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

#include "InterfaceList.hpp"
#include "../Global.hpp"
#include <QDataStream>
#include <QFile>
#include <QMessageBox>

InterfaceList* InterfaceList::interfacelist = nullptr;

InterfaceList::InterfaceList()
{
    open();
}

InterfaceList::~InterfaceList()
{
    save();
    for (int i = 0; i < this->List.count(); i++) {
        delete this->List.at(i);
    }
}

InterfaceList* InterfaceList::instance()
{
    if (interfacelist == nullptr) {
        interfacelist = new InterfaceList;
    }
    return interfacelist;
}

void InterfaceList::release()
{
    if (interfacelist != nullptr) {
        delete interfacelist;
    }
    interfacelist = nullptr;
}

bool InterfaceList::hasPredefinedIP(QString hwaddress) const
{
    bool RetVal = false;

    for (int i = 0; i < this->List.size(); i++) {
        Interface* Interface = this->List.at(i);
        if ((Interface->hardwareAddress() == hwaddress) && (Interface->predefinedIPcount() != 0)) {
            RetVal = true;
            break;
        }
    }

    return RetVal;
}

Interface* InterfaceList::interface(QString hwaddress) const
{
    Interface* Interface = nullptr;

    // Lookup in the Interface list to find the one which owns the HW address
    // Maybe it does not exist
    for (int i = 0; i < this->List.size(); i++) {
        class Interface* TmpInterface = this->List.at(i);
        if (TmpInterface->hardwareAddress() == hwaddress) {
            Interface = TmpInterface;
            break;
        }
    }

    return Interface;
}

void InterfaceList::addInterface(Interface* interface)
{
    this->List.append(interface);
}

void InterfaceList::deleteInterface(Interface* interface)
{
    qsizetype Index = this->List.indexOf(interface);
    // Should never return -1. Just a safety check
    if (Index != -1) {
        delete this->List.takeAt(Index);
    }
}

QList<Interface*> InterfaceList::interfaceList() const
{
    return this->List;
}

bool InterfaceList::save() const
{
    // Backup data file
    // Check existence because it is cleaner, but it is quite useless...
    if (QFile::exists(FOLCO_BACKUP_FILENAME)) {
        QFile::remove(FOLCO_BACKUP_FILENAME);
    }
    if (QFile::exists(FOLCO_FILENAME)) {
        QFile::rename(FOLCO_FILENAME, FOLCO_BACKUP_FILENAME);
    }

    // Open file
    QFile File(FOLCO_FILENAME);
    if (!File.open(QIODeviceBase::WriteOnly)) {
        QMessageBox::critical(
            nullptr, QString("%1 error").arg(APPLICATION_NAME), QString("Error while saving the file %1. No data saved.").arg(FOLCO_FILENAME));
        return false;
    }

    // Create associated stream
    QDataStream Stream(&File);

    // Number of Interface to write
    qint32 Count = this->List.size();

    // Header
    Stream << QString(FOLCO_SIGNATURE).toUtf8() << (qint32)DATA_VERSION << Count;

    // Save every interface
    for (int i = 0; i < Count; i++) {
        this->List.at(i)->save(Stream);
    }

    // Check stream status
    if (Stream.status() != QDataStream::Ok) {
        QMessageBox::critical(nullptr, QString("%1 error").arg(APPLICATION_NAME), QString("Error while saving the file %1.").arg(FOLCO_FILENAME));
        return false;
    }

    return true;
}

void InterfaceList::open()
{
    // Early return if there is no stored file
    if (!QFile::exists(FOLCO_FILENAME)) {
        return;
    }

    // Try to open the file
    QFile File(FOLCO_FILENAME);
    if (!File.open(QIODeviceBase::ReadOnly)) {
        QMessageBox::critical(
            nullptr, QString("%1 error").arg(APPLICATION_NAME), QString("Error while opening the file %1. No data read.").arg(FOLCO_FILENAME));
        return;
    }

    // Read file content
    QDataStream Stream(&File);
    QByteArray  Signature;
    qint32      Version;
    qint32      Count;

    // Check signature
    Stream >> Signature;
    if ((Stream.status() != QDataStream::Ok) || (QString::fromUtf8(Signature) != FOLCO_SIGNATURE)) {
        QMessageBox::critical(
            nullptr, QString("%1 error").arg(APPLICATION_NAME), QString("Error while opening the file %1. Wrong signature.").arg(FOLCO_FILENAME));
        return;
    }

    // Check version
    Stream >> Version;
    if ((Stream.status() != QDataStream::Ok) || (Version > HIGHER_SUPPORTED_VERSION)) {
        QMessageBox::critical(nullptr,
                              QString("%1 error").arg(APPLICATION_NAME),
                              QString("Error while opening the file %1. The data file is too recent, please upgrade your software.").arg(FOLCO_FILENAME));
        return;
    }

    // Read number of saved Interfaces
    Stream >> Count;
    if (Stream.status() != QDataStream::Ok) {
        QMessageBox::critical(
            nullptr, QString("%1 error").arg(APPLICATION_NAME), QString("Error while opening the file %1. Invalid IP count.").arg(FOLCO_FILENAME));
        return;
    }

    // Create the interface and add them to the list
    // The Interface objects check the stream status while extracting themselves from the stream
    for (int i = 0; i < Count; i++) {
        Interface* Interface = new class Interface(Stream, Version);
        this->List.append(Interface);
    }
}

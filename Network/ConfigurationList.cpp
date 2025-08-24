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

#include "ConfigurationList.hpp"
#include "../Global.hpp"
#include <QDataStream>
#include <QFile>
#include <QMessageBox>

ConfigurationList* ConfigurationList::configurationlist = nullptr;

ConfigurationList::ConfigurationList()
{
    open();
}

ConfigurationList::~ConfigurationList()
{
    save();
    for (int i = 0; i < this->List.count(); i++) {
        delete this->List.at(i);
    }
}

ConfigurationList* ConfigurationList::instance()
{
    if (configurationlist == nullptr) {
        configurationlist = new ConfigurationList;
    }
    return configurationlist;
}

void ConfigurationList::release()
{
    if (configurationlist != nullptr) {
        delete configurationlist;
    }
    configurationlist = nullptr;
}

bool ConfigurationList::hasPredefinedIP(QString hwaddress) const
{
    bool RetVal = false;

    for (int i = 0; i < this->List.size(); i++) {
        Configuration* Configuration = this->List.at(i);
        if ((Configuration->hardwareAddress() == hwaddress) && (Configuration->predefinedIPcount() != 0)) {
            RetVal = true;
            break;
        }
    }

    return RetVal;
}

Configuration* ConfigurationList::configuration(QString hwaddress) const
{
    Configuration* Configuration = nullptr;

    // Lookup in the Configuration list to find the one which owns the HW address
    // Maybe it does not exist
    for (int i = 0; i < this->List.size(); i++) {
        class Configuration* TmpConfiguration = this->List.at(i);
        if (TmpConfiguration->hardwareAddress() == hwaddress) {
            Configuration = TmpConfiguration;
            break;
        }
    }

    return Configuration;
}

void ConfigurationList::addConfiguration(Configuration* configuration)
{
    this->List.append(configuration);
}

void ConfigurationList::deleteConfiguration(Configuration* configuration)
{
    qsizetype Index = this->List.indexOf(configuration);
    // Should never return -1. Just a safety check
    if (Index != -1) {
        delete this->List.takeAt(Index);
    }
}

QList<Configuration*> ConfigurationList::configurationList() const
{
    return this->List;
}

bool ConfigurationList::save() const
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

    // Number of Configurations to write
    qint32 Count = this->List.size();

    // Header
    Stream << QString(FOLCO_SIGNATURE).toUtf8() << (qint32)DATA_VERSION << Count;

    // Save every Configuration
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

void ConfigurationList::open()
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

    // Read number of Configurations
    Stream >> Count;
    if (Stream.status() != QDataStream::Ok) {
        QMessageBox::critical(
            nullptr, QString("%1 error").arg(APPLICATION_NAME), QString("Error while opening the file %1. Invalid IP count.").arg(FOLCO_FILENAME));
        return;
    }

    // Create the Configurations and add them to the list
    // The Configuration objects check the stream status while extracting themselves from the stream
    for (int i = 0; i < Count; i++) {
        Configuration* Configuration = new class Configuration(Stream, Version);
        this->List.append(Configuration);
    }
}

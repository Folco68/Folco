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

#include "Logger.hpp"
#include <QDate>
#include <QProcess>
#include <QTime>

Logger* Logger::logger = nullptr;

Logger* Logger::instance()
{
    if (logger == nullptr) {
        logger = new Logger;
    }
    return logger;
}

void Logger::release()
{
    if (logger != nullptr) {
        delete logger;
        logger = nullptr;
    }
}

Logger::Logger()
    : FirstLogEntry(true)
{
    QList List(QProcess::systemEnvironment().toList());
    while (!List.isEmpty()) {
        addLogEntry(List.takeFirst());
    }
}

QString Logger::log() const
{
    return this->Log;
}

void Logger::addLogEntry(QString entry)
{
    if (this->FirstLogEntry) {
        this->FirstLogEntry = false;
    }
    else {
        this->Log.append("\n");
    }

    QString NewLine = QString("[%1] %2").arg(QTime::currentTime().toString("hh:mm:ss.zzz"), entry);
    this->Log.append(NewLine);
    emit newLogEntry(NewLine);
}

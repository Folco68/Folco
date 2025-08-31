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

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <QString>

class Logger
{
    //--------------------------------------------------------------------------
    //                             Singleton stuff                             -
    //--------------------------------------------------------------------------

  public:
    static Logger* instance();
    static void    release();

  private:
    Logger();
    static Logger* logger;

    //--------------------------------------------------------------------------
    //                                 Logger                                  -
    //--------------------------------------------------------------------------

  public:
    void           addLogEntry(QString entry);
    QString        log() const;

  private:
    QString Log;
    bool    FirstLogEntry;
};

#endif // LOGGER_HPP

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

#ifndef GLOBAL_HPP
#define GLOBAL_HPP

// QCoreApplication settings (for QSettings) + app name
#define ORGANIZATION_NAME       "FolcoSoft"
#define APPLICATION_NAME        "Folco"
#define APPLICATION_DESCRIPTION "Fast IP changer"

#define FOLCO_FILENAME           "Folco.pdi"
#define FOLCO_BACKUP_FILENAME    "Folco.bak"
#define FOLCO_SIGNATURE          "FOLCO_BY_MARTIAL_DEMOLINS"
#define DATA_VERSION             2
#define HIGHER_SUPPORTED_VERSION 2

#endif // GLOBAL_HPP

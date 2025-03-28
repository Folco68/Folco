/*******************************************************************************
 *                                                                             *
 * Folco - Program allowing to quickly change the IPv4 address of an interface *
 *                     Copyright (C) 2024 Martial Demolins                     *
 *                                                                             *
 *    This program is free software: you can redistribute it and/or modify     *
 *    it under the terms of the GNU General Public License as published by     *
 *      the Free Software Foundation, either version 3 of the License, or      *
 *                      at your option) any later version                      *
 *                                                                             *
 *       This program is distributed in the hope that it will be useful        *
 *       but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
 *                 GNU General Public License for more details                 *
 *                                                                             *
 *      You should have received a copy of the GNU General Public License      *
 *     along with this program.  If not, see <https://www.gnu.org/licenses     *
 *                                                                             *
 ******************************************************************************/

#ifndef BEFORERELEASE_HPP
#define BEFORERELEASE_HPP

//
// Some strings that need an update before a commit or a release
//
// Just before committing:
// Update Changelog.txt with the git changelog
// Update the version in this file
// Update TODO if necessary
//
#define APPLICATION_VERSION_STR "0.9.5"
#define POSITION_STR            "Service Engineer (S2), Market France/BeNeLux, Tetra Pak"
#define COPYRIGHT_STR           "(c)2024-2025 Martial Demolins"
#define EMAIL_PROFESSIONAL      "martial.demolins@tetrapak.com"
#define EMAIL_PERSONAL          "martial.demolins@gmail.com"

#endif // BEFORERELEASE_HPP

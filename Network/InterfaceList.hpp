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

#ifndef INTERFACE_LIST_HPP
#define INTERFACE_LIST_HPP

#include "Interface.hpp"
#include "PredefinedIP.hpp"
#include <QDataStream>
#include <QList>
#include <QString>

class InterfaceList
{
  public:
    static InterfaceList* instance();
    static void       release();

    Interface*        interface(QString hwaddress) const;
    bool              hasPredefinedIP(QString hwaddress) const;
    void              addInterface(Interface* interface);
    void              deleteInterface(Interface* interface);
    QList<Interface*> interfaceList() const;

  private:
    // Singleton stuff
    static InterfaceList* interfacelist;
    InterfaceList();
    ~InterfaceList();

    void open();
    bool save() const;

    QList<Interface*> List;
};

#endif // INTERFACE_LIST_HPP

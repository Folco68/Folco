#ifndef INTERFACE_LIST_HPP
#define INTERFACE_LIST_HPP

#include "Interface.hpp"
#include "PredefinedIP.hpp"
#include <QList>
#include <QString>

class InterfaceList
{
  public:
    static InterfaceList* instance();
    static void       release();

    Interface* interface(QString hwaddress) const;
    bool       hasPredefinedIP(QString hwaddress) const;
    void       addInterface(Interface* interface);

  private:
    static InterfaceList* interfacelist;
    QList<Interface*>     List;

    InterfaceList();
};

#endif // INTERFACE_LIST_HPP

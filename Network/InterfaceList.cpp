#include "InterfaceList.hpp"

InterfaceList* InterfaceList::interfacelist = nullptr;

InterfaceList::InterfaceList()
{
    // TODO: Open the file
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

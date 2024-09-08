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

bool InterfaceList::hasConfiguration(QString hwaddress) const
{
    for (int i = 0; i < this->List.size(); i++) {
        if (this->List.at(i)->hardwareAddress() == hwaddress) {
            return true;
        }
    }
    return false;
}

Interface* InterfaceList::interface(QString hwaddress) const
{
    for (int i = 0; i < this->List.size(); i++) {
        if (this->List.at(i)->hardwareAddress() == hwaddress) {
            return this->List.at(i);
        }
    }
    return nullptr;
}

void InterfaceList::addPredefinedIP(QString hwaddress, PredefinedIP ip)
{
    Interface* Interface = nullptr;

    for (int i = 0; i < this->List.size(); i++) {
        class Interface* TmpInterface = this->List.at(i);
        if (TmpInterface->hardwareAddress() == hwaddress) {
            Interface = TmpInterface;
            break;
        }
    }

    if (Interface == nullptr) {
        Interface = new class Interface(hwaddress);
        this->List.append(Interface);
    }

    Interface->addPredefinedIP(ip);
}

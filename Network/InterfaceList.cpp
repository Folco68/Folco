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
    bool RetVal = false;

    for (int i = 0; i < this->List.size(); i++) {
        if (this->List.at(i)->hardwareAddress() == hwaddress) {
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

void InterfaceList::addPredefinedIP(QString hwaddress, PredefinedIP ip)
{
    Interface* Interface = nullptr;

    // Check if the interface already exists in the list of preconfigured devices
    for (int i = 0; i < this->List.size(); i++) {
        class Interface* TmpInterface = this->List.at(i);
        if (TmpInterface->hardwareAddress() == hwaddress) {
            Interface = TmpInterface;
            break;
        }
    }

    // If the interface does not exist yet, create and add it to the list
    if (Interface == nullptr) {
        Interface = new class Interface(hwaddress);
        this->List.append(Interface);
    }

    // Add the predefined IP to the interface
    Interface->addPredefinedIP(ip);
}

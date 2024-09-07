#include "IFCfgList.hpp"

IFCfgList* IFCfgList::ifcfglist = nullptr;

IFCfgList::IFCfgList()
{
    // Open the file
}

IFCfgList* IFCfgList::instance()
{
    if (ifcfglist == nullptr) {
        ifcfglist = new IFCfgList;
    }
    return ifcfglist;
}

void IFCfgList::release()
{
    if (ifcfglist != nullptr) {
        delete ifcfglist;
    }
    ifcfglist = nullptr;
}

bool IFCfgList::hasConfiguration(QString HardwareAddress) const
{
    for (int i = 0; i < this->List.size(); i++) {
        if (List.at(i)->hardwareAddress() == HardwareAddress) {
            return true;
        }
    }
    return false;
}

IFCfg* IFCfgList::ifCfg(QString HardwareAddress) const
{
    for (int i = 0; i < List.size(); i++) {
        if (List.at(i)->hardwareAddress() == HardwareAddress) {
            return List.at(i);
        }
    }
    return nullptr;
}

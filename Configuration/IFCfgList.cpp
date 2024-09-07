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

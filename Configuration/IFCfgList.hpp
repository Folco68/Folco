#ifndef IFCFGLIST_HPP
#define IFCFGLIST_HPP

#include "IFCfg.hpp"
#include <QList>
#include <QString>

class IFCfgList
{
  public:
    static IFCfgList* instance();
    static void       release();

    bool   hasConfiguration(QString HardwareAddress) const;
    IFCfg* ifCfg(QString HardwareAddress) const;

  private:
    static IFCfgList* ifcfglist;
    IFCfgList();

    // An IFCfg must not exist without at least one entry
    QList<IFCfg*> List;
};

#endif // IFCFGLIST_HPP

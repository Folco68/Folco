#ifndef IFCFG_HPP
#define IFCFG_HPP

#include <QList>
#include <QString>

class IFCfg
{
  public:
    IFCfg(QString HardwareAddress);
    void addCfg(QString IPaddress, QString NetworkMask, QString Gateway);
    void removeCfg(QString IPaddress, QString NetworkMask, QString Gateway);

  private:
    QString        HardwareAddress;
    QList<QString> IPaddress;
    QList<QString> NetworkMask;
    QList<QString> Gateway;
};

#endif // IFCFG_HPP

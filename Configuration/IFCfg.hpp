#ifndef IFCFG_HPP
#define IFCFG_HPP

#include <QList>
#include <QString>

class IFCfg
{
  public:
    IFCfg(QString HardwareAddress);
    void                  addCfg(QString IPaddress, QString NetworkMask, QString Gateway);
    void                  removeCfg(QString IPaddress, QString NetworkMask, QString Gateway);
    QString               hardwareAddress() const;
    QList<QList<QString>> configuration() const;

  private:
    QString               HardwareAddress;
    QList<QList<QString>> Configuration;
};

#endif // IFCFG_HPP

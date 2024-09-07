#ifndef IFCFG_HPP
#define IFCFG_HPP

#include <QList>
#include <QString>
#include <QVector>

class IFCfg
{
  public:
    IFCfg(QString HardwareAddress);
    void                    addCfg(QString IPaddress, QString NetworkMask, QString Gateway);
    void                    removeCfg(QString IPaddress, QString NetworkMask, QString Gateway);
    QString                 hardwareAddress() const;
    QList<QVector<QString>> configuration() const;

  private:
    QString                 HardwareAddress;
    QList<QVector<QString>> Configuration;
};

#endif // IFCFG_HPP

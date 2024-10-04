#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include "PredefinedIP.hpp"
#include <QDataStream>
#include <QList>
#include <QString>

class Interface
{
  public:
    Interface(QString hwaddress);
    Interface(QDataStream& stream);
    void                 addPredefinedIP(PredefinedIP* ip);
    void                 removePredefinedIP(PredefinedIP* ip);
    int                  predefinedIPcount() const;
    QString              hardwareAddress() const;
    QList<PredefinedIP*> predefinedIPlist() const;
    void                 clearContent();
    void                 save(QDataStream& stream);

  private:
    QString              HardwareAddress;
    QList<PredefinedIP*> PredefinedIPlist;
};

#endif // INTERFACE_HPP

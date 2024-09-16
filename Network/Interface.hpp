#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include "PredefinedIP.hpp"
#include <QList>
#include <QString>

class Interface
{
  public:
    Interface(QString hwaddress);
    void                addPredefinedIP(PredefinedIP ip);
    void                removePredefinedIP(PredefinedIP ip);
    int                 predefinedIPcount() const;
    QString             hardwareAddress() const;
    QList<PredefinedIP> predefinedIPlist() const;
    void                clearContent();

  private:
    QString             HardwareAddress;
    QList<PredefinedIP> PredefinedIPlist;
};

#endif // INTERFACE_HPP

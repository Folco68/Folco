#ifndef PREDEFINED_IP_HPP
#define PREDEFINED_IP_HPP

#include <QString>

class Interface;

class PredefinedIP
{
  public:
    PredefinedIP(Interface* parent, QString Name, QString ip, QString mask, QString gateway);
    Interface* parent() const;
    QString    name() const;
    QString ipAddress() const;
    QString networkMask() const;
    QString gateway() const;
    bool    operator==(PredefinedIP ip) const;

  private:
    Interface* Parent; // Keep a track of the parent to find the interface to configure when this Predefined IP is selected in the tray i,con context menu
    QString    Name;
    QString    IPaddress;
    QString    NetworkMask;
    QString    Gateway;
};

#endif // PREDEFINED_IP_HPP

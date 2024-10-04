#ifndef PREDEFINED_IP_HPP
#define PREDEFINED_IP_HPP

#include <QDataStream>
#include <QString>

class Interface;

class PredefinedIP
{
  public:
    PredefinedIP(Interface* parent, QString Name, QString ip, QString mask, QString gateway);
    PredefinedIP(Interface* parent, QDataStream& stream);
    Interface* parent() const;
    QString    name() const;
    QString    ipAddress() const;
    QString    networkMask() const;
    QString    gateway() const;
    bool       operator==(PredefinedIP ip) const;
    void       save(QDataStream& stream);

  private:
    // Keep a track of the interface which own this settings, to find the interface to configure when this Predefined IP is selected in the tray icon context menu
    Interface* Parent;
    QString    Name;
    QString    IPaddress;
    QString    NetworkMask;
    QString    Gateway;
};

#endif // PREDEFINED_IP_HPP

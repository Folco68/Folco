#ifndef PREDEFINED_IP_HPP
#define PREDEFINED_IP_HPP

#include <QString>

class PredefinedIP
{
  public:
    PredefinedIP() {};
    PredefinedIP(QString Name, QString ip, QString mask, QString gateway);
    QString name() const;
    QString ipAddress() const;
    QString networkMask() const;
    QString gateway() const;
    bool    isValid() const;
    void    setPredefinedIP(QString Name, QString ip, QString mask, QString gateway);
    bool    operator==(PredefinedIP ip) const;

  private:
    QString Name;
    QString IPaddress;
    QString NetworkMask;
    QString Gateway;
};

#endif // PREDEFINED_IP_HPP

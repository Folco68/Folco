#ifndef PREDEFINED_IP_HPP
#define PREDEFINED_IP_HPP

#include <QString>

class PredefinedIP
{
  public:
    PredefinedIP() {};
    PredefinedIP(QString ip, QString mask, QString gateway);
    QString ipAddress() const;
    QString networkMask() const;
    QString gateway() const;
    bool    isValid() const;
    void    setPredefinedIP(QString ip, QString mask, QString gateway);

  private:
    QString IPaddress;
    QString NetworkMask;
    QString Gateway;
};

#endif // PREDEFINED_IP_HPP

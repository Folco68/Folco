#ifndef IPVALIDATOR_HPP
#define IPVALIDATOR_HPP

#include <QValidator>

class IPvalidator: public QValidator
{
  public:
    explicit IPvalidator(QObject* parent = nullptr);
    QValidator::State validate(QString& input, int&) const override;
};

#endif // IPVALIDATOR_HPP

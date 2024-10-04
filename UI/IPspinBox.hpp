#ifndef IP_SPIN_BOX_HPP
#define IP_SPIN_BOX_HPP

#include <QFocusEvent>
#include <QKeyEvent>
#include <QSpinBox>
#include <QWidget>

class IPspinBox: public QSpinBox
{
  public:
    IPspinBox(QWidget* parent);
    void focusInEvent(QFocusEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void setNextByteBox(IPspinBox* box);

  private:
    IPspinBox* NextByteBox;
};

#endif // IP_SPIN_BOX_HPP

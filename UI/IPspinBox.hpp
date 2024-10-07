#ifndef IP_SPIN_BOX_HPP
#define IP_SPIN_BOX_HPP

#include <QFocusEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QSpinBox>
#include <QWidget>

class IPspinBox: public QSpinBox
{
  public:
    IPspinBox(QWidget* parent);
    void focusInEvent(QFocusEvent* event) override;      // Select all when grabbing kbd focus
    void mouseReleaseEvent(QMouseEvent* event) override; // Select all on click
    void keyReleaseEvent(QKeyEvent* event) override;     // Netxt byte when dot is hit

    void setNextByteBox(IPspinBox* box);

  private:
    IPspinBox* NextByteBox;
};

#endif // IP_SPIN_BOX_HPP

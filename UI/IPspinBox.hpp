#ifndef IP_SPIN_BOX_HPP
#define IP_SPIN_BOX_HPP

#include <QDragEnterEvent>
#include <QDropEvent>
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
    void setPreviousByteBox(IPspinBox* box);

    void dragEnterEvent(QDragEnterEvent* event) override;
    void dropEvent(QDropEvent* event) override;

  private:
    IPspinBox* NextByteBox;
    IPspinBox* PreviousByteBox;
};

#endif // IP_SPIN_BOX_HPP

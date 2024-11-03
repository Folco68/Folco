#ifndef IPLINEEDIT_HPP
#define IPLINEEDIT_HPP

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QLineEdit>
#include <QString>
#include <QWidget>

class IPlineEdit: public QLineEdit
{
  public:
    IPlineEdit(QWidget* parent);
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dropEvent(QDropEvent* event) override;

  private:
    bool isIPvalid(QString ip) const;
};

#endif // IPLINEEDIT_HPP

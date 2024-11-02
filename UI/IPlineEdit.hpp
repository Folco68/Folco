#ifndef IPLINEEDIT_HPP
#define IPLINEEDIT_HPP

#include "IPvalidator.hpp"
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QLineEdit>
#include <QString>
#include <QWidget>


class IPlineEdit: public QLineEdit
{
  public:
    IPlineEdit(QWidget* parent);
    ~IPlineEdit();
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dropEvent(QDropEvent* event) override;

  private:
    //    void paste();
    bool isIPvalid(QString ip) const;
    IPvalidator* validator;
};

#endif // IPLINEEDIT_HPP

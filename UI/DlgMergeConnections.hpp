#ifndef DLGMERGECONNECTIONS_HPP
#define DLGMERGECONNECTIONS_HPP

#include <QDialog>

namespace Ui {
class DlgMergeConnections;
}

class DlgMergeConnections : public QDialog
{
    Q_OBJECT

public:
    explicit DlgMergeConnections(QWidget *parent = nullptr);
    ~DlgMergeConnections();

private:
    Ui::DlgMergeConnections *ui;
};

#endif // DLGMERGECONNECTIONS_HPP

#ifndef DLG_LOG_HPP
#define DLG_LOG_HPP

#include <QDialog>

namespace Ui {
class DlgLog;
}

class DlgLog : public QDialog
{
    Q_OBJECT

public:
    static void showDlgLog();

private:
    static DlgLog *dlglog;
    DlgLog();
    ~DlgLog();
    void scrollToTheEnd();

private:
    Ui::DlgLog *ui;
};

#endif // DLG_LOG_HPP

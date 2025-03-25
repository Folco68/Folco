#ifndef WINDOWLOG_HPP
#define WINDOWLOG_HPP

#include <QWidget>

namespace Ui {
class WindowLog;
}

class WindowLog : public QWidget
{
    Q_OBJECT

public:
    static void showWindowLog();

private:
    static WindowLog *windowlog;
    WindowLog();
    ~WindowLog();
    void scrollToTheEnd();

private:
    Ui::WindowLog *ui;
};

#endif // WINDOWLOG_HPP

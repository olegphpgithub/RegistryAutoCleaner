#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class SimpleTimer;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void ClearRegistry();
    void AutoClearStartStop();
    void TimeoutExceeded();

private:
    Ui::MainWindow *ui;
    bool bAutoClear;
    SimpleTimer *stimer;
};

#endif // MAINWINDOW_H

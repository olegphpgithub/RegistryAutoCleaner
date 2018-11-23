#ifndef SIMPLETIMER_H
#define SIMPLETIMER_H

#include <QThread>

class SimpleTimer : public QThread
{
    Q_OBJECT
    void run();
    int iTimeOut;
public:
    explicit SimpleTimer(QObject *parent = 0);
    ~SimpleTimer();
    void setTimeout(int timeOut);
};

#endif // SIMPLETIMER_H

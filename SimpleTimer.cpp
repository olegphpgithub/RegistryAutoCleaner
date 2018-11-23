#include "SimpleTimer.h"

SimpleTimer::SimpleTimer()
{
    qDebug() << "SimpleTimer constructor";
}

SimpleTimer::~SimpleTimer()
{
    qDebug() << "SimpleTimer constructor";
}

void SimpleTimer::setTimeout(int timeOut)
{
    iTimeOut = timeOut;
}

void SimpleTimer::run() {

    int i = 3;
    sleep(i);

}

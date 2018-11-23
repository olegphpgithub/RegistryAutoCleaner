#include "SimpleTimer.h"

#include <QtCore>
#include <QString>
#include <QtDebug>

SimpleTimer::SimpleTimer(QObject *parent) : QThread(parent)
{
    qDebug() << "SimpleTimer constructor";
}

SimpleTimer::~SimpleTimer()
{
    qDebug() << "SimpleTimer destructor";
}

void SimpleTimer::setTimeout(int timeOut)
{
    iTimeOut = timeOut;
}

void SimpleTimer::run() {

    sleep(iTimeOut);

}

#include "timerrssi.h"

TimerRSSI::TimerRSSI(QObject *parent)
    :   QObject(parent)
{
    connect(&timer, SIGNAL(timeout()), SLOT (Timer()));
    timer.start(5000);
}

void TimerRSSI::Timer()
{
    qDebug() << "=/=/=/=/=/=/=/=/=/=/=/ " << "RSSItimer!   " << " =/=/=/=/=/=/=/=/=/=/=/";
    emit doTimer();
}

TimerRSSI::~TimerRSSI()
{

}

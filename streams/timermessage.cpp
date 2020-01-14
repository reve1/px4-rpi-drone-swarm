#include "timermessage.h"

TimerMessage::TimerMessage(QObject *parent)
    :   QObject(parent)
{
    connect(&timer, SIGNAL(timeout()), SLOT (Timer()));
}

void TimerMessage::StartTimer()
{
    timer.start(1000);
}

void TimerMessage::Timer()
{
    qDebug() << "====================== " << "TimerMessage!" << " ======================";
    emit doTimer();
}

TimerMessage::~TimerMessage()
{

}

#ifndef TIMERMESSAGE_H
#define TIMERMESSAGE_H
#include <QDebug>
#include <QTimer>

class TimerMessage : public QObject
{
    Q_OBJECT

public:
    TimerMessage();
    ~TimerMessage();
    QTimer timer;

public slots:
    void Timer();
    void StartTimer();

signals:
    void doTimer();
};

#endif // TIMERMESSAGE_H

#ifndef TIMERRSSI_H
#define TIMERRSSI_H
#include <QDebug>
#include <QTimer>

class TimerRSSI : public QObject
{
    Q_OBJECT

public:
    TimerRSSI(QObject *parent = nullptr);
    ~TimerRSSI();
    QTimer timer;

public slots:
    void Timer();

signals:
    void doTimer();
};

#endif // TIMERRSSI_H

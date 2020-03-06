#ifndef UDPCLIENT_H
#define UDPCLIENT_H
#include <QObject>
#include <QtNetwork>
#include <QTimer>

class QUdpSocket;
class UdpClient : public QObject
{
    Q_OBJECT
public:
    explicit UdpClient(QObject *parent = nullptr);
private slots:
    void startBroadcasting();
    void broadcastDatagram();
private:
    QUdpSocket *udpSocket = nullptr;
    QTimer timer;
    int messageNo = 1;
};
#endif // UDPCLIENT_H






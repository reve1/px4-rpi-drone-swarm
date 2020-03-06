#ifndef UDPSERVER_H
#define UDPSERVER_H
#include <QObject>
#include <QtNetwork>

class QUdpSocket;
class UdpServer : public QObject
{
    Q_OBJECT
public:
    explicit UdpServer(QObject *parent = nullptr);
private slots:
    void processPendingDatagrams();
private:
    QUdpSocket *udpSocket = nullptr;
};
#endif // UDPSERVER_H






#ifndef UDPSERVER_H
#define UDPSERVER_H
#include <QObject>
#include <QtNetwork>

#include <streams/filewrite.h>

class QUdpSocket;
class UdpServer : public QObject
{
    Q_OBJECT
public:
    explicit UdpServer(QObject *parent = nullptr);
signals:
    void ReceivedRemoteVehicleInfo(unsigned long,double,double,float,float,int,int,float);
private slots:
    void processPendingDatagrams();
private:
    QUdpSocket *udpSocket = nullptr;
    QString data;
};
#endif // UDPSERVER_H






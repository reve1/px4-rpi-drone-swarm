#ifndef UDPCLIENT_H
#define UDPCLIENT_H
#include <QObject>
#include <QtNetwork>
#include <QTimer>

#include <streams/filewrite.h>

class QUdpSocket;
class UdpClient : public QObject
{
    Q_OBJECT

public:
    explicit UdpClient(QObject *parent = nullptr);

private slots:
    void startBroadcasting();
    void broadcastDatagram();
    void sendLocalVehicleInfo(const unsigned long &UUID,
                              const double &Lat,
                              const double &Lon,
                              const float &Alt,
                              const float &AMSL,
                              const int &GPS_num,
                              const int &GPS_fix_type,
                              const float &Battery);

private:
    QUdpSocket *udpSocket = nullptr;
    QTimer timer;
    QString data;
    int messageNo = 1;


};
#endif // UDPCLIENT_H

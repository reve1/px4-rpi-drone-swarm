#include "udpclient.h"

UdpClient::UdpClient(QObject *parent): QObject(parent)
{
    udpSocket = new QUdpSocket(this);
    //connect(startButton, &QPushButton::clicked, this, &UdpClient::startBroadcasting);
    //connect(quitButton, &QPushButton::clicked, this, &UdpClient::close);
    //connect(&timer, &QTimer::timeout, this, &UdpClient::broadcastDatagram);
    //startBroadcasting();
}

void UdpClient::startBroadcasting()
{
    timer.start(1000);
}

void UdpClient::broadcastDatagram()
{
    QByteArray datagram = "1/23/-1,047/" + QByteArray::number(messageNo)+"/";
    udpSocket->writeDatagram(datagram, QHostAddress::Broadcast, 6666);
    //qDebug() << "Отправлена датаграмма: " << messageNo;
    data = "Отправлена датаграмма: " + messageNo;
    FileWrite::WriteFromClass(6,data);
    ++messageNo;
}

void UdpClient::sendLocalVehicleInfo(const unsigned long &UUID,
                                     const double &Lat,
                                     const double &Lon,
                                     const float &Alt,
                                     const float &AMSL,
                                     const int &GPS_num,
                                     const int &GPS_fix_type,
                                     const float &Battery,
                                     const int &Lider,
                                     const int &Number,
                                     const int &Formation,
                                     const float &angle_yaw)
{
    QString x = QString::number(UUID);
    QByteArray datagram = x.toUtf8() + "/"
            + QByteArray::number(Lat) + "/"
            + QByteArray::number(Lon) + "/"
            + QByteArray::number(Alt) + "/"
            + QByteArray::number(AMSL) + "/"
            + QByteArray::number(GPS_num) + "/"
            + QByteArray::number(GPS_fix_type) + "/"
            + QByteArray::number(Battery) + "/"
            + QByteArray::number(Lider) + "/"
            + QByteArray::number(Number) + "/"
            + QByteArray::number(Formation) + "/"
            + QByteArray::number(angle_yaw) + "/";
    udpSocket->writeDatagram(datagram, QHostAddress::Broadcast, 6666);
    //qDebug() << "Отправлена датаграмма: " << datagram;
}

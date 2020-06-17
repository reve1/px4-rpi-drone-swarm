#include "udpclient.h"
#include <sstream>

UdpClient::UdpClient(QObject *parent): QObject(parent)
{
    udpSocket = new QUdpSocket(this);
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
                                     const float &angle_yaw,
                                     const int &flightMode,
                                     const QDateTime &VehicleTimeStamp)
{
    QByteArray datagram = QString::number(UUID).toUtf8() + "/"
            + QByteArray::number(Lat,'g',17) + "/"
            + QByteArray::number(Lon,'g',17) + "/"
            + QByteArray::number(Alt) + "/"
            + QByteArray::number(AMSL) + "/"
            + QByteArray::number(GPS_num) + "/"
            + QByteArray::number(GPS_fix_type) + "/"
            + QByteArray::number(Battery) + "/"
            + QByteArray::number(Lider) + "/"
            + QByteArray::number(Number) + "/"
            + QByteArray::number(Formation) + "/"
            + QByteArray::number(angle_yaw) + "/"
            + QByteArray::number(flightMode) + "/"
            + VehicleTimeStamp.toString().toUtf8() + "/";
    udpSocket->writeDatagram(datagram, QHostAddress::Broadcast, 6666);
    //qDebug() << "Отправлена датаграмма: " << datagram;
}

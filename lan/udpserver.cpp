#include "udpserver.h"

UdpServer::UdpServer(QObject *parent): QObject(parent)
{
    udpSocket = new QUdpSocket(this);
    udpSocket->bind(6666, QUdpSocket::ShareAddress);
    connect(udpSocket, SIGNAL(readyRead()),
            this, SLOT(processPendingDatagrams()));
}

void UdpServer::processPendingDatagrams()
{
    QByteArray datagram;
    while (udpSocket->hasPendingDatagrams())
    {
        datagram.resize(int(udpSocket->pendingDatagramSize()));
        udpSocket->readDatagram(datagram.data(), datagram.size());
        qDebug() << "Получена датаграмма: " << datagram.constData();
        QString string = datagram.constData();
        data = "Получена датаграмма: " + string.simplified();
        FileWrite::WriteFromClass(7,data);
        string.chop(1);
        QStringList list = string.split('/');

        unsigned long UUID = list.at(0).simplified().toLong();
        double LAT = list.at(1).simplified().toDouble();
        double LON = list.at(2).simplified().toDouble();
        float ALT = list.at(3).simplified().toFloat();
        float AMSL = list.at(4).simplified().toFloat();
        int GPS = list.at(5).simplified().toInt();
        int GPS_fix_type = list.at(6).simplified().toInt();
        float Battery = list.at(7).simplified().toFloat();
        int Lider = list.at(8).simplified().toInt();
        int Number = list.at(9).simplified().toInt();
        int Formation = list.at(10).simplified().toInt();
        float angle_yaw = list.at(11).simplified().toFloat();
        int flightMode = list.at(12).simplified().toFloat();

        list.clear();
        emit ReceivedRemoteVehicleInfo(UUID,LAT,LON,ALT,AMSL,GPS,GPS_fix_type,Battery,Lider,Number,Formation,angle_yaw,flightMode);
    }
}

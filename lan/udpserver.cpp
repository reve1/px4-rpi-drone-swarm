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
    qDebug() << udpSocket->peerAddress().toIPv4Address();
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
        double LON = list.at(1).simplified().toDouble();
        double LAT = list.at(2).simplified().toDouble();
        float ALT = list.at(3).simplified().toFloat();
        float AMSL = list.at(4).simplified().toFloat();
        int GPS = list.at(5).simplified().toInt();
        int GPS_fix_type = list.at(6).simplified().toInt();
        float Battery = list.at(7).simplified().toFloat();
        //qDebug() << list.at(0).simplified();
        //data = "1 параметр: " + list.at(0).simplified();
        //FileWrite::WriteFromClass(7,data);
        //qDebug() << list.at(1).simplified();
        //data = "2 параметр: " + list.at(1).simplified();
        //FileWrite::WriteFromClass(7,data);
        //qDebug() << list.at(2).simplified();
        //data = "3 параметр: " + list.at(2).simplified();
        //FileWrite::WriteFromClass(7,data);
        //qDebug() << list.at(3).simplified();
        //data = "4 параметр: " + list.at(3).simplified();
        //FileWrite::WriteFromClass(7,data);

        list.clear();
        emit ReceivedRemoteVehicleInfo(UUID,LAT,LON,ALT,AMSL,GPS,GPS_fix_type,Battery);
    }
}

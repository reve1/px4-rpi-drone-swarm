#include "udpclient.h"

UdpClient::UdpClient(QObject *parent): QObject(parent)
{
    udpSocket = new QUdpSocket(this);
    //connect(startButton, &QPushButton::clicked, this, &UdpClient::startBroadcasting);
    //connect(quitButton, &QPushButton::clicked, this, &UdpClient::close);
    connect(&timer, &QTimer::timeout, this, &UdpClient::broadcastDatagram);
    startBroadcasting();
}

void UdpClient::startBroadcasting()
{
    timer.start(1000);
}

void UdpClient::broadcastDatagram()
{
    QByteArray datagram = "1/23/-1,047/" + QByteArray::number(messageNo)+"/";
    udpSocket->writeDatagram(datagram, QHostAddress::Broadcast, 6666);
    qDebug() << "Отправлена датаграмма: " << messageNo;
    data = "Отправлена датаграмма: " + messageNo;
    FileWrite::WriteFromClass(6,data);
    ++messageNo;
}

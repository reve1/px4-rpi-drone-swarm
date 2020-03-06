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
    qDebug() << "Отправлена датаграмма: " << messageNo;
    QByteArray datagram = "Broadcast сообщение " + QByteArray::number(messageNo);
    udpSocket->writeDatagram(datagram, QHostAddress::Broadcast, 6666);
    ++messageNo;
}

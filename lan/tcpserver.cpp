#include "tcpserver.h"
#include <QDebug>
#include <QCoreApplication>

TcpServer::TcpServer(QObject *parent) : QObject(parent)
{
    mTcpServer = new QTcpServer(this);

    connect(mTcpServer, &QTcpServer::newConnection, this, &TcpServer::slotNewConnection);

    if(!mTcpServer->listen(QHostAddress::Any, 6000)){
        qDebug() << "TCP сервер не запущен";
    } else {
        qDebug() << "TCP сервер запущен";
    }
}

void TcpServer::slotNewConnection()
{
    mTcpSocket = mTcpServer->nextPendingConnection();

    mTcpSocket->write("Успешное подключение к серверу PX4\r\n");

    connect(mTcpSocket, &QTcpSocket::readyRead, this, &TcpServer::slotServerRead);
    connect(mTcpSocket, &QTcpSocket::disconnected, this, &TcpServer::slotClientDisconnected);
}

void TcpServer::slotServerRead()
{
    while(mTcpSocket->bytesAvailable()>0)
    {
        QByteArray array = mTcpSocket->readAll();
        mTcpSocket->write(array);
        qDebug() << array;
    }
}

void TcpServer::slotClientDisconnected()
{
    mTcpSocket->close();
}

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
    while (udpSocket->hasPendingDatagrams()) {
        datagram.resize(int(udpSocket->pendingDatagramSize()));
        udpSocket->readDatagram(datagram.data(), datagram.size());
        qDebug() << "Получена датаграмма: " << datagram.constData();
    }
}






/*
TcpServer::TcpServer(QObject *parent) :
    QObject(parent),
    firstSocket(NULL)
{
    server = new QTcpServer(this);
    qDebug() << "Запущен TCP сервер = " << server->listen(QHostAddress::Any, 6666);
    connect(server, SIGNAL(newConnection()), this, SLOT(incommingConnection())); // подключаем сигнал "новое подключение" к нашему обработчику подключений
}
void TcpServer::incommingConnection() // обработчик подключений
{
    QTcpSocket * socket = server->nextPendingConnection(); // получаем сокет нового входящего подключения
    connect(socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(stateChanged(QAbstractSocket::SocketState))); // делаем обработчик изменения статуса сокета
    if (!firstSocket) { // если у нас нет "вещающего", то данное подключение становится вещающим
        connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead())); // подключаем входящие сообщения от вещающего на наш обработчик
        socket->write("server"); // говорим ему что он "вещает"
        firstSocket = socket; // сохраняем себе"
        qDebug() << "this one is server";
    }
    else { // иначе говорим подключенному что он "получатель"
        socket->write("client");
        sockets << socket;
    }
}
void TcpServer::readyRead() // обработчик входящих сообщений от "вещающего"
{
    QObject * object = QObject::sender(); // далее и ниже до цикла идет преобразования "отправителя сигнала" в сокет, дабы извлечь данные
    if (!object)
        return;
    qDebug() << "[1]";
    QTcpSocket * socket = static_cast<QTcpSocket *>(object);
    QByteArray arr =  socket->readAll();
    qDebug() << arr.simplified();
    // на самом деле весь верхний код можно было заменить на firstSocket, но я выдирал код из другого проекта, и переписывать мне лень :)
    foreach(QTcpSocket *socket, sockets) { // пишем входящие данные от "вещающего" получателям
        if (socket->state() == QTcpSocket::ConnectedState)
            socket->write(arr);
    }
}
void TcpServer::stateChanged(QAbstractSocket::SocketState state) // обработчик статуса, нужен для контроля за "вещающим"
{
    QObject * object = QObject::sender();
    if (!object)
        return;
    QTcpSocket * socket = static_cast<QTcpSocket *>(object);
    qDebug() << state;
    if (socket == firstSocket && state == QAbstractSocket::UnconnectedState)
        firstSocket = NULL;
}
*/

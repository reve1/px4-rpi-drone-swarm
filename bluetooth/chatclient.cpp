#include "bluetooth/chatclient.h"

#include <QtCore/qmetaobject.h>

ChatClient::ChatClient(QObject *parent)
    :   QObject(parent)
{
    data = "Отработал конструктор класса ChatClient.";
    fw->WriteFromClass(2, data);
}

ChatClient::~ChatClient()
{
    stopClient();
    data = "Отработал деструктор класса ChatClient.";
    fw->WriteFromClass(2, data);
}

void ChatClient::startClient(const QBluetoothServiceInfo &remoteService)
{
    if (socket)
        return;

    socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol);
    qDebug() << "Создан socket";
    data = "Создан socket";
    fw->WriteFromClass(2, data);
    socket->connectToService(remoteService, QIODevice::ReadWrite);
    qDebug() << "Подключение к " + socket->peerAddress().toString() + " выполнено";
    data = "Подключение к " + socket->peerAddress().toString() + " выполнено";
    fw->WriteFromClass(2, data);
    connect(socket, &QBluetoothSocket::readyRead, this, &ChatClient::readSocket);
    connect(socket, &QBluetoothSocket::connected, this, QOverload<>::of(&ChatClient::connected));
    connect(socket, &QBluetoothSocket::disconnected, this, &ChatClient::disconnected);
    connect(socket, QOverload<QBluetoothSocket::SocketError>::of(&QBluetoothSocket::error),
            this, &ChatClient::onSocketErrorOccurred);
}

void ChatClient::stopClient()
{
    delete socket;
    socket = nullptr;
    data = "Клиент остановлен";
    fw->WriteFromClass(2, data);
}

void ChatClient::readSocket()
{
    if (!socket)
        return;

    while (socket->canReadLine()) {
        QByteArray line = socket->readLine();
        emit messageReceived(socket->peerName(),
                             QString::fromUtf8(line.constData(), line.length()));
        data = "Получено от " + socket->peerName() +" сообщение: " + QString::fromUtf8(line.constData(), line.length());
        fw->WriteFromClass(3, data.simplified());
    }
}

void ChatClient::sendMessage(const QString &message)
{
    QByteArray text = message.toUtf8() + '\n';
    socket->write(text);
    qCritical() << "Информация передана: " + message.simplified(); //ECHO
    data = "Отправлено на " + socket->peerName() +" сообщение: " + message;
    fw->WriteFromClass(3, data);
}

void ChatClient::onSocketErrorOccurred(QBluetoothSocket::SocketError error)
{
    if (error == QBluetoothSocket::NoSocketError)
        return;

    QMetaEnum metaEnum = QMetaEnum::fromType<QBluetoothSocket::SocketError>();
    QString errorString = socket->peerName() + QLatin1Char(' ')
            + metaEnum.valueToKey(error) + QLatin1String(" occurred");

    emit socketErrorOccurred(errorString);
    data = "Ошибка соединения: " + errorString;
    fw->WriteFromClass(2, data);
}

void ChatClient::connected()
{
    emit connected(socket->peerName());
}

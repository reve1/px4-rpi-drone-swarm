#include "bluetooth/chatclient.h"
#include <QtCore/qmetaobject.h>

ChatClient::ChatClient(QObject *parent)
    :   QObject(parent)
{
    data = "Отработал конструктор класса ChatClient.";
    FileWrite::WriteFromClass(2, data);
}

ChatClient::~ChatClient()
{
    stopClient();
    data = "Отработал деструктор класса ChatClient.";
    FileWrite::WriteFromClass(2, data);
}

void ChatClient::startClient(const QBluetoothServiceInfo &remoteService)
{
    if (socket)
        return;

    socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol);
    socket->setPreferredSecurityFlags(QBluetooth::NoSecurity);
    qDebug() << "Создан socket";
    data = "Создан socket";
    FileWrite::WriteFromClass(2, data);
    socket->connectToService(remoteService, QIODevice::ReadWrite);
    qDebug() << "Подключение к " << socket->peerAddress().toString() << " выполнено";
    data = "Подключение к " + socket->peerAddress().toString() + " выполнено";
    FileWrite::WriteFromClass(2, data);
    connect(socket, &QBluetoothSocket::readyRead, this, &ChatClient::readSocket);
    connect(socket, &QBluetoothSocket::connected, this, QOverload<>::of(&ChatClient::connected));
    connect(socket, &QBluetoothSocket::disconnected, this, &ChatClient::disconnected);
    connect(socket, QOverload<QBluetoothSocket::SocketError>::of(&QBluetoothSocket::error),
            this, &ChatClient::onSocketErrorOccurred);
    //connect(this, SIGNAL(messageReceived_reply()), this, SLOT(sendClicked()));
}

void ChatClient::stopClient()
{
    delete socket;
    socket = nullptr;
    data = "Клиент остановлен";
    FileWrite::WriteFromClass(2, data);
}

void ChatClient::readSocket()
{
    if (!socket)
        return;

    while (socket->canReadLine()) {
        QByteArray line = socket->readLine();
        emit messageReceived(socket->peerName(), QString::fromUtf8(line.constData(), line.length()));
        qCritical() << "Получено от " << socket->peerName() <<  " сообщение: " << line.simplified();
        data = "Получено от " + socket->peerName() + " сообщение: " + QString::fromUtf8(line.constData(), line.length());
        FileWrite::WriteFromClass(3, data.simplified());
        //emit messageReceived_reply();
    }
}

void ChatClient::sendMessage(const QString &message)
{
    QByteArray text = message.toUtf8() + '\n';
    socket->write(text);
    qCritical() << "Отправлено на " << socket->peerName() <<  " сообщение: "  << message.simplified();
    data = "Отправлено на " + socket->peerName() +" сообщение: " + message.simplified();
    FileWrite::WriteFromClass(3, data);
}

void ChatClient::onSocketErrorOccurred(QBluetoothSocket::SocketError error)
{
    if (error == QBluetoothSocket::NoSocketError)
        return;

    QMetaEnum metaEnum = QMetaEnum::fromType<QBluetoothSocket::SocketError>();
    QString errorString = socket->peerName() + QLatin1Char(' ')
            + metaEnum.valueToKey(error) + QLatin1String(" occurred");

    emit socketErrorOccurred(errorString);
    qCritical() << "Ошибка соединения: " << errorString;
    data = "Ошибка соединения: " + errorString;
    FileWrite::WriteFromClass(2, data);
}

void ChatClient::connected()
{
    qDebug() << "Подключен к: " + socket->peerName();
    data = "Подключен к: " + socket->peerName();
    FileWrite::WriteFromClass(3, data);
    emit connected(socket->peerName());
}

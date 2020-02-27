#include "chat.h"
#include "bluetooth/chatserver.h"
#include "bluetooth/chatclient.h"

#include <QtCore/qdebug.h>

#include <QtBluetooth/qbluetoothdeviceinfo.h>
#include <QtBluetooth/qbluetoothlocaldevice.h>
#include <QtBluetooth/qbluetoothuuid.h>

Chat::Chat(QObject *parent)
    :   QObject(parent),  currentAdapterIndex(0)
{
    server = new ChatServer (this);

    connect(tm, SIGNAL(doTimer()), this, SLOT(sendClicked()));
    connect(server, SIGNAL(clientConnected(QString)), this, SLOT(clientConnected(QString)));
    connect(server, SIGNAL(clientDisconnected(QString)), this, SLOT(clientDisconnected(QString)));
    //connect(server, SIGNAL(messageReceived(QString,QString)), this, SLOT(showMessage(QString,QString)));
    //connect(this, SIGNAL(sendMessage(QString)), server, SLOT(sendMessage(QString)));
    connect(this, SIGNAL(SEND()), tm, SLOT(StartTimer())); //убрать эту хрень после потока

    server->startServer();
    localName = QBluetoothLocalDevice().name();

    qDebug() << "Отработал конструктор класса Chat";
    data = "Отработал конструктор класса Chat.";
    fw->WriteFromClass(3, data);
}

Chat::~Chat()
{
    qDeleteAll(clients);
    delete server;
    data = "Отработал деструктор класса Chat.";
    fw->WriteFromClass(3, data);
}

void Chat::clientConnected(const QString &name)
{
    qDebug() << "Подключился клиент: " + name;
    data = "Подключился клиент: " + name;
    fw->WriteFromClass(3, data);
}

void Chat::clientDisconnected(const QString &name)
{
    qDebug() << "Отключился клиент: " + name;
    data = "Отключился клиент: " + name;
    fw->WriteFromClass(3, data);
}

void Chat::connected(const QString &name)
{
    qDebug() << "Подключен к: " + name;
    data = "Подключен к: " + name;
    fw->WriteFromClass(3, data);
    emit SEND(); //убрать эту хрень после потока
}

void Chat::reactOnSocketError(const QString &error)
{
    qDebug() << "Ошибка: " + error;
    data = "Ошибка: " + error;
    fw->WriteFromClass(3, data);
    emit socketError();
    tm->timer.stop();
}

void Chat::clientDisconnected()
{
    ChatClient *client = qobject_cast<ChatClient *>(sender());
    if (client) {
        clients.removeOne(client);
        client->deleteLater();
    }
}

void Chat::sendClicked() //убрать эту хрень после потока
{
    emit sendMessage(message);
    qCritical() << "Информация передана: " + message.simplified(); //ECHO
}

//void Chat::showMessage(const QString &sender, const QString &message) //убрать эту хрень после потока
//{
//    qCritical() << "Принято сообщение от: " + sender + ", с текстом " + message.simplified();
//    //emit sendMessage(message1); //убрать эту хрень после потока
//}

void Chat::connectClicked(QBluetoothServiceInfo info)
{
    QBluetoothServiceInfo service = info;

    qDebug() << "Подключение к сервису: " << service.serviceName() << "на" << service.device().address();

    ChatClient *client = new ChatClient(this);
    qDebug() << "Подключение...";

    //connect(client, &ChatClient::messageReceived, this, &Chat::showMessage);
    connect(client, &ChatClient::disconnected, this, QOverload<>::of(&Chat::clientDisconnected));
    connect(client, QOverload<const QString &>::of(&ChatClient::connected), this, &Chat::connected);
    connect(client, &ChatClient::socketErrorOccurred, this, &Chat::reactOnSocketError);
    connect(this, &Chat::sendMessage, client, &ChatClient::sendMessage);
    qDebug() << "Клиент стартовал";
    client->startClient(service);

    clients.append(client);
}

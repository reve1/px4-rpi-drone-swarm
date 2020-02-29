#include "bluetooth/chatserver.h"

#include <QtBluetooth/qbluetoothserver.h>
#include <QtBluetooth/qbluetoothsocket.h>

//static const QLatin1String serviceUuid("00001101-0000-1000-8000-00805F9B34FB");
static const QLatin1String serviceUuid("e8e10f95-1a70-4b27-9ccf-02010264e9c8");

ChatServer::ChatServer(QObject *parent)
    :   QObject(parent)
{
    connect(this, SIGNAL(messageReceived_reply(QString)), this, SLOT(sendMessage(QString)));
    data = "Отработал конструктор класса ChatServer.";
    fw->WriteFromClass(1, data);
}

ChatServer::~ChatServer()
{
    stopServer();
    data = "Отработал деструктор класса ChatServer.";
    fw->WriteFromClass(1, data);
}

void ChatServer::startServer(const QBluetoothAddress& localAdapter)
{
    stopServer();
    if (rfcommServer)
        return;

    rfcommServer = new QBluetoothServer(QBluetoothServiceInfo::RfcommProtocol, this);
    rfcommServer->setSecurityFlags(QBluetooth::Security::NoSecurity);
    connect(rfcommServer, &QBluetoothServer::newConnection,
            this, QOverload<>::of(&ChatServer::clientConnected));
    bool result = rfcommServer->listen(localAdapter);
    if (!result) {
        qWarning() << "Невозможно привязать сервер к " << localAdapter.toString();
        data = "Невозможно привязать сервер к " + localAdapter.toString();
        fw->WriteFromClass(1, data);
        return;
    }

    serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceRecordHandle, (uint)0x00010010);

    QBluetoothServiceInfo::Sequence profileSequence;
    QBluetoothServiceInfo::Sequence classId;
    classId << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::SerialPort));
    classId << QVariant::fromValue(quint16(0x100));
    profileSequence.append(QVariant::fromValue(classId));
    serviceInfo.setAttribute(QBluetoothServiceInfo::BluetoothProfileDescriptorList,
                             profileSequence);

    classId.clear();
    classId << QVariant::fromValue(QBluetoothUuid(serviceUuid));
    classId << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::SerialPort));

    serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceClassIds, classId);

    serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceName, tr("Bt Chat Server"));
    serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceDescription, tr("Example bluetooth chat server"));
    serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceProvider, tr("qt-project.org"));

    serviceInfo.setServiceUuid(QBluetoothUuid(serviceUuid));

    QBluetoothServiceInfo::Sequence publicBrowse;
    publicBrowse << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::PublicBrowseGroup));
    serviceInfo.setAttribute(QBluetoothServiceInfo::BrowseGroupList,
                             publicBrowse);

    QBluetoothServiceInfo::Sequence protocolDescriptorList;
    QBluetoothServiceInfo::Sequence protocol;
    protocol << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::L2cap));
    protocolDescriptorList.append(QVariant::fromValue(protocol));
    protocol.clear();
    protocol << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::Rfcomm))
             << QVariant::fromValue(quint8(rfcommServer->serverPort()));
    protocolDescriptorList.append(QVariant::fromValue(protocol));
    serviceInfo.setAttribute(QBluetoothServiceInfo::ProtocolDescriptorList,
                             protocolDescriptorList);

    serviceInfo.registerService(localAdapter);

    qWarning() << "Старт сервера на " << localAdapter.toString();
    data = "Старт сервера на " + localAdapter.toString();
    fw->WriteFromClass(1, data);

}

void ChatServer::stopServer()
{
    serviceInfo.unregisterService();
    qDeleteAll(clientSockets);
    delete rfcommServer;
    rfcommServer = nullptr;
    data = "Стоп сервера Bluetooth.";
    fw->WriteFromClass(1, data);
}

void ChatServer::sendMessage(const QString &message)
{
    QByteArray text = message.toUtf8() + '\n';

    for (QBluetoothSocket *socket : qAsConst(clientSockets))
    {
        qCritical() << "Отправлено на " << socket->peerName() <<  " сообщение: "  << message.simplified();
        data = "Отправлено на: " + socket->peerName() +" сообщение: " + message;
        fw->WriteFromClass(3, data);
        socket->write(text);
    }

}

void ChatServer::clientConnected()
{
    QBluetoothSocket *socket = rfcommServer->nextPendingConnection();
    if (!socket)
        return;

    connect(socket, &QBluetoothSocket::readyRead, this, &ChatServer::readSocket);
    connect(socket, &QBluetoothSocket::disconnected, this, QOverload<>::of(&ChatServer::clientDisconnected));
    clientSockets.append(socket);

    emit clientConnected(socket->peerName());

    qDebug() << "Подключился клиент: " << socket->peerName() << " адресс: " << socket->peerAddress().toString();
    data = "Подключился клиент " + socket->peerAddress().toString();
    fw->WriteFromClass(1, data);
}

void ChatServer::clientDisconnected()
{
    QBluetoothSocket *socket = qobject_cast<QBluetoothSocket *>(sender());
    if (!socket)
        return;

    emit clientDisconnected(socket->peerName());
    clientSockets.removeOne(socket);
    socket->deleteLater();
    qDebug() << "Отключился клиент: " << socket->peerName() << " адресс: " << socket->peerAddress().toString();
    data = "Отключился клиент " + socket->peerAddress().toString();
    fw->WriteFromClass(1, data);
}

void ChatServer::readSocket()
{
    QBluetoothSocket *socket = qobject_cast<QBluetoothSocket *>(sender());
    if (!socket)
        return;

    while (socket->canReadLine()) {
        QByteArray line = socket->readLine().trimmed();
        emit messageReceived(socket->peerName(), QString::fromUtf8(line.constData(), line.length()));
        emit messageReceived_reply("REPLY");
        qCritical() << "Получено от " << socket->peerName() <<  " сообщение: " << line.simplified();
        data = "Получено от: " + socket->peerName() +" сообщение: " + line.simplified();
        fw->WriteFromClass(3, data.simplified());
    }
}

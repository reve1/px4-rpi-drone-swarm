#ifndef BLUETOOTHCHAT_H
#define BLUETOOTHCHAT_H

#include <QBluetoothServiceInfo>
#include <QBluetoothServer>
#include <QBluetoothSocket>
#include <QBluetoothHostInfo>
#include <QBluetoothLocalDevice>
#include <QBluetoothServiceDiscoveryAgent>

#include <QDebug>

#include <bluetooth/chatclient.h>
#include <bluetooth/chatserver.h>
#include <streams/filewrite.h>
#include <streams/timermessage.h>


QT_USE_NAMESPACE

class Chat : public QObject
{
    Q_OBJECT
public:
    explicit Chat(QObject *parent = nullptr);
    ~Chat();

signals:
    void sendMessage(const QString &message);
    void SEND();
    void socketError();

private slots:
    void connectClicked(QBluetoothServiceInfo);
    void sendClicked();

    void showMessage(const QString &sender, const QString &message);

    void clientConnected(const QString &name);
    void clientDisconnected(const QString &name);
    void clientDisconnected();
    void connected(const QString &name);
    void reactOnSocketError(const QString &error);

private:
    int adapterFromUserSelection() const;
    int currentAdapterIndex = 0;

    ChatServer *server;
    QList<ChatClient *> clients;
    QList<QBluetoothHostInfo> localAdapters;
    QBluetoothServiceDiscoveryAgent *discoveryServiceAgent = new QBluetoothServiceDiscoveryAgent;

    QString localName;

    FileWrite *fw;
    TimerMessage *tm = new TimerMessage;
    QString data;

    QString message = "ECHO";                   // запроc
    QString message1 = "REPLY";                 // ответ
};

#endif // BLUETOOTHCHAT_H

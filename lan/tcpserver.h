#ifndef TCPSERVER_H
#define TCPSERVER_H
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QByteArray>
#include <QDataStream>
class TcpServer : public QObject
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = 0); // конструктор
signals:
public slots:
    void incommingConnection(); // обработчик входящего подключения
    void readyRead(); // обработчик входящих данных
    void stateChanged(QAbstractSocket::SocketState stat); // обработчик изменения состояния вещающего сокета (он нам важен, дабы у нас всегда был кто-то, кто будет вещать
private:
    QTcpServer *server; // указатель на сервер
    QList<QTcpSocket *> sockets; // получатели данных
    QTcpSocket *firstSocket; // вещатель
};
#endif // TCPSERVER_H

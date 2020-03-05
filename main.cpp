#include <QCoreApplication>
#include <QtNetwork>
#include <qbluetooth.h>
#include <qbluetoothdevicediscoveryagent.h>
#include <QBluetoothLocalDevice>
#include <QLoggingCategory>

#include <streams/filewrite.h>
#include <streams/timerrssi.h>
#include <streams/timermessage.h>
#include <bluetooth/chat.h>
#include <bluetooth/bluetoothdiscovery.h>
#include <lan/tcpserver.h>
#include <model/model.h>

#if !defined (Q_OS_WIN)
#include <vehicle/vehicle.h>
#endif

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    //QLoggingCategory::setFilterRules(QStringLiteral("qt.bluetooth* = true"));
    FileWrite *fw = new FileWrite;
    Model *md = new Model;
    Chat *bchat = new Chat;
    TcpServer server;
#if !defined (Q_OS_WIN)
    Vehicle *vh = new Vehicle;
#endif


    //BluetoothDiscovery *bd = new BluetoothDiscovery;
    //QObject::connect(bd, SIGNAL(deviceFound(QBluetoothServiceInfo)), bchat, SLOT(connectClicked(QBluetoothServiceInfo)));
    //QObject::connect(bchat, SIGNAL(socketError()), bd, SLOT(StartServiceDiscovery()));
    //bd->StartServiceDiscovery();

    return a.exec();
}

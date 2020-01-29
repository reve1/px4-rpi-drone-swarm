#include <QCoreApplication>
#include <qbluetooth.h>
#include <qbluetoothdevicediscoveryagent.h>
#include <QBluetoothLocalDevice>
#include <QLoggingCategory>

#include <streams/filewrite.h>
#include <streams/timerrssi.h>
#include <streams/timermessage.h>
#include <bluetooth/chat.h>
#include <bluetooth/bluetoothdiscovery.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    //QLoggingCategory::setFilterRules(QStringLiteral("qt.bluetooth* = true"));
    FileWrite *fw = new FileWrite;
    Chat *bch = new Chat;
    BluetoothDiscovery *bd = new BluetoothDiscovery;

    QObject::connect(bd, SIGNAL(deviceFound(QBluetoothServiceInfo)), bch, SLOT(connectClicked(QBluetoothServiceInfo)));

    bd->StartServiceDiscovery();

    return a.exec();
}

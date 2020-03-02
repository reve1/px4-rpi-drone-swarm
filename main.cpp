/*
sudo hciconfig 0 sspmode 0
sudo rfcomm connect hci0 B8:27:EB:89:C5:44 8
*/

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
#include <model/model.h>

#if !defined (_WIN)
#include <vehicle/vehicle.h>
#endif

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    //QLoggingCategory::setFilterRules(QStringLiteral("qt.bluetooth* = true"));
    FileWrite *fw = new FileWrite;
    Model *md = new Model;
    Chat *bchat = new Chat;

    //BluetoothDiscovery *bd = new BluetoothDiscovery;
    //QObject::connect(bd, SIGNAL(deviceFound(QBluetoothServiceInfo)), bchat, SLOT(connectClicked(QBluetoothServiceInfo)));
    //QObject::connect(bchat, SIGNAL(socketError()), bd, SLOT(StartServiceDiscovery()));
    //bd->StartServiceDiscovery();

    Vehicle *vh = new Vehicle;

    return a.exec();
}

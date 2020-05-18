#include <QCoreApplication>
#include <QThread>
#include <QtNetwork>
#include <QLoggingCategory>
#include <QBluetoothLocalDevice>

#include <qbluetooth.h>
#include <qbluetoothdevicediscoveryagent.h>

#include <streams/filewrite.h>
#include <streams/timerrssi.h>
#include <streams/timermessage.h>
#include <bluetooth/chat.h>
#include <bluetooth/bluetoothdiscovery.h>
#include <lan/udpserver.h>
#include <lan/udpclient.h>
#include <model/model.h>

#if !defined (Q_OS_WIN)
#include <vehicle/vehicle.h>
#endif

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    //QLoggingCategory::setFilterRules(QStringLiteral("qt.bluetooth* = true"));

    Model *md = new Model;
    Chat *bchat = new Chat;
    FileWrite fw;
    UdpServer *server = new UdpServer;
    UdpClient *client = new UdpClient;

#if !defined (Q_OS_WIN)
    Vehicle *vh = new Vehicle;
    QThread *VhThred = new QThread;
    vh->moveToThread(VhThred);
    VhThred->start();
    QObject::connect(vh, SIGNAL(LocalVehicleInfo(unsigned long,double,double,float,float,int,int)), md, SLOT(setLocalVehicleInfo(unsigned long,double,double,float,float,int,int)));
    QObject::connect(vh, SIGNAL(LocalVehicleInfo(unsigned long,double,double,float,float,int,int)), client, SLOT(sendLocalVehicleInfo(unsigned long,double,double,float,float,int,int)));
    QObject::connect(server, SIGNAL(ReceivedRemoteVehicleInfo(unsigned long,double,double,float,float,int,int)), md, SLOT(setRemoteVehicleInfo(unsigned long,double,double,float,float,int,int)));
    QObject::connect(md, SIGNAL(newCoordSet(double, double)), vh, SLOT(folowMeSetCoord(double, double)));
    QObject::connect(VhThred, SIGNAL(started()), vh, SLOT(Run()));
#endif

    //BluetoothDiscovery *bd = new BluetoothDiscovery;
    //QObject::connect(bd, SIGNAL(deviceFound(QBluetoothServiceInfo)), bchat, SLOT(connectClicked(QBluetoothServiceInfo)));
    //QObject::connect(bchat, SIGNAL(socketError()), bd, SLOT(StartServiceDiscovery()));
    //bd->StartServiceDiscovery();

    return a.exec();
}

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

    Model *md = new Model;
    UdpServer *server = new UdpServer;
    UdpClient *client = new UdpClient;
    FileWrite fw;

#if !defined (Q_OS_WIN)
    QThread *VhThred = new QThread;
    Vehicle *vh = new Vehicle;
    vh->moveToThread(VhThred);
    VhThred->start();
    QObject::connect(vh, SIGNAL(LocalUUID(unsigned long)),
                     md, SLOT(setLocalUUID(unsigned long)));
    QObject::connect(vh, SIGNAL(LocalVehiclePositionInfo(unsigned long,double,double,float,float)),
                     md, SLOT(setLocalVehiclePositionInfo(unsigned long,double,double,float,float)));
    QObject::connect(vh, SIGNAL(LocalVehicleGPSInfo(unsigned long,int,int)),
                     md, SLOT (setLocalVehicleGPSInfo(unsigned long,int,int)));
    QObject::connect(vh, SIGNAL(LocalVehicleBatteryInfo(unsigned long,float)),
                     md, SLOT (setLocalVehicleBatteryInfo(unsigned long,float)));
    QObject::connect(vh, SIGNAL(LocalVehicleAngle(unsigned long,float)),
                     md, SLOT (setLocalVehicleAngle(unsigned long,float)));
    QObject::connect(vh, SIGNAL(LocalVehicleFlightMode(unsigned long,int)),
                     md, SLOT (setLocalVehicleFlightMode(unsigned long,int)));
    QObject::connect(md, SIGNAL(sendLocalVehicleInfo(unsigned long, double, double,float,float,int,int,float,int,int,int,float,int,QDateTime)),
                     client, SLOT(sendLocalVehicleInfo(unsigned long,double,double,float,float,int,int,float,int,int,int,float,int,QDateTime)));
    //QObject::connect(vh, SIGNAL(LocalVehicleInfo(unsigned long,double,double,float,float,int,int)), client, SLOT(sendLocalVehicleInfo(unsigned long,double,double,float,float,int,int)));
    QObject::connect(server, SIGNAL(ReceivedRemoteVehicleInfo(unsigned long,double,double,float,float,int,int,float,int,int,int,float,int,QDateTime)),
                     md, SLOT(setRemoteVehicleInfo(unsigned long,double,double,float,float,int,int,float,int,int,int,float,int,QDateTime)));
    QObject::connect(md, SIGNAL(goToPosition(double,double,float,float)),
                     vh, SLOT(fly(double,double,float,float)),Qt::DirectConnection);
    QObject::connect(md, SIGNAL(Takeoff()),
                     vh, SLOT(setTakeOff()),Qt::DirectConnection);
    QObject::connect(md, SIGNAL(ReturnToLaunch()),
                     vh, SLOT(setReturnToLaunch()),Qt::DirectConnection);
    QObject::connect(md, SIGNAL(Land()),
                     vh, SLOT(setLand()),Qt::DirectConnection);
    QObject::connect(VhThred, SIGNAL(started()),
                     vh, SLOT(Run()));
#endif

    //QLoggingCategory::setFilterRules(QStringLiteral("qt.bluetooth* = true"));
    //Chat *bchat = new Chat;
    //BluetoothDiscovery *bd = new BluetoothDiscovery;
    //QObject::connect(bd, SIGNAL(deviceFound(QBluetoothServiceInfo)), bchat, SLOT(connectClicked(QBluetoothServiceInfo)));
    //QObject::connect(bchat, SIGNAL(socketError()), bd, SLOT(StartServiceDiscovery()));
    //bd->StartServiceDiscovery();

    return a.exec();
}

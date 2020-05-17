#include "bluetoothdiscovery.h"

#if defined (Q_OS_ANDROID)
#include <QtAndroidExtras/QtAndroid>
static const QLatin1String reverseUuid("c8e96402-0102-cf9c-274b-701a950fe1e8");
#endif

static const QLatin1String serviceUuid("e8e10f95-1a70-4b27-9ccf-02010264e9c8");

//static const QLatin1String bluetoothAddress("9C:E0:63:BD:A8:88"); //Note 8 (android)
//static const QLatin1String bluetoothAddress("B8:27:EB:89:C5:44"); //RPi
//static const QLatin1String bluetoothAddress("A4:17:31:36:3E:16"); //Notebook (Linux)
static const QLatin1String bluetoothAddress("68:94:23:39:C4:36");

BluetoothDiscovery::BluetoothDiscovery(QObject *parent) : QObject(parent)
{
    connect(discoveryServiceAgent, SIGNAL(serviceDiscovered(QBluetoothServiceInfo)), this, SIGNAL(deviceFound(QBluetoothServiceInfo)));
    connect(discoveryServiceAgent, SIGNAL(serviceDiscovered(QBluetoothServiceInfo)), this, SLOT(discoveryService_discovered()));
    connect(discoveryServiceAgent, SIGNAL(serviceDiscovered(QBluetoothServiceInfo)), this, SLOT(discoveryService_stoped()));
    connect(discoveryServiceAgent, SIGNAL(finished()), this, SLOT(discoveryService_finished()));
    connect(discoveryDeviceAgent, SIGNAL(finished()), this, SLOT(discoveryDevice_finished()));
#if !defined (Q_OS_ANDROID)
    connect(discoveryServiceAgent, SIGNAL(finished()), this, SLOT(StartServiceDiscovery()));
    connect(discoveryDeviceAgent, SIGNAL(finished()), this, SLOT(StartDeviceDiscovery()));
#endif
    //(discoveryDeviceAgent, SIGNAL(finished()), this, SLOT(StartDeviceDiscovery()));
    //connect(discoveryDeviceAgent, SIGNAL(error(QBluetoothDeviceDiscoveryAgent::Error error)), this, SLOT(StartDeviceDiscovery()));

#if !defined (Q_OS_WIN) //не работает на ОС Windows
    TimerRSSI *timerrssi = new TimerRSSI;
    connect(timerrssi, SIGNAL(doTimer()), this, SLOT(UpdateRSSI()));
#endif

    //connect(discoveryServiceAgent, SIGNAL(finished()), this, SLOT(StartServiceDiscovery())); //если не нашел иши дальше... возможно это и не нужно...
    //connect(localDevice, SIGNAL(hostModeStateChanged(QBluetoothLocalDevice::HostMode)), this, SLOT(SetHostDiscoverable())); //позволяет обновлять "видимость в сети" но на андроиде не работает, а надо ли?
    SetHostDiscoverable();
    StartDeviceDiscovery();
    qDebug() << "Отработал конструктор класса BluetoothDiscovery.";
    data = "Отработал конструктор класса BluetoothDiscovery.";
    FileWrite::WriteFromClass(4, data);
}

void BluetoothDiscovery::SetHostDiscoverable()
{

#if (!defined (_WIN) || !defined (_ANDROID))
    localDevice->setHostMode(QBluetoothLocalDevice::HostPoweredOff);
    localDevice->setHostMode(QBluetoothLocalDevice::HostDiscoverable);
#endif
    localDevice->setHostMode(QBluetoothLocalDevice::HostDiscoverable);
    qDebug() << "Устройство " << localDevice->name() << " обнаружимо в сети";
    data = "Устройство " + localDevice->name() + " обнаружимо в сети";
    FileWrite::WriteFromClass(4, data);
}

void BluetoothDiscovery::StartServiceDiscovery()
{
    discoveryServiceAgent->clear();
#ifdef Q_OS_ANDROID
    //if (QtAndroid::androidSdkVersion() >= 23)
    //    discoveryServiceAgent->setUuidFilter(QBluetoothUuid(reverseUuid));
    //else
    //    discoveryServiceAgent->setUuidFilter(QBluetoothUuid(serviceUuid));
#else
    discoveryServiceAgent->setUuidFilter(QBluetoothUuid(serviceUuid));
#endif
    discoveryServiceAgent->setRemoteAddress(QBluetoothAddress(bluetoothAddress));
    discoveryServiceAgent->setUuidFilter(QBluetoothUuid(serviceUuid));
    discoveryServiceAgent->start(QBluetoothServiceDiscoveryAgent::FullDiscovery);
    qDebug() << "Начато сканирование сервисов";
    data = "Начато сканирование сервисов";
    FileWrite::WriteFromClass(4, data);
}

void BluetoothDiscovery::StartDeviceDiscovery()
{
    discoveryDeviceAgent->stop();
    discoveryDeviceAgent->start();
    qDebug() << "Начато сканирование устройств";
    data = "Начато сканирование устройств";
    FileWrite::WriteFromClass(4, data);
}

void BluetoothDiscovery::UpdateRSSI()
{
    for( int i = 0; i < discoveryDeviceAgent->discoveredDevices().count(); i++ )
    {
        if (discoveryDeviceAgent->discoveredDevices().at(i).rssi()!= 0)
        {
            qDebug() << "Устройство" << discoveryDeviceAgent->discoveredDevices().at(i).name() << "RSSI" << discoveryDeviceAgent->discoveredDevices().at(i).rssi();
            data = "Устройство " + discoveryDeviceAgent->discoveredDevices().at(i).name() + "RSSI " + QString::number(discoveryDeviceAgent->discoveredDevices().at(i).rssi());
            FileWrite::WriteFromClass(4, data);
        }
    }
}

void BluetoothDiscovery::discoveryService_finished()
{
#if !defined (Q_OS_ANDROID)
    qDebug() << "Cканирование сервисов выполнено";
    data = "Cканирование сервисов выполнено";
    FileWrite::WriteFromClass(4, data);
#endif
}

void BluetoothDiscovery::discoveryService_discovered()
{
    qDebug() << "Обнаружен нужный сервис";
    data = "Обнаружен нужный сервис";
    FileWrite::WriteFromClass(4, data);
}

void BluetoothDiscovery::discoveryDevice_finished()
{
#if !defined (Q_OS_ANDROID)
    qDebug() << "Cканирование устройств выполнено";
    data = "Cканирование устройств выполнено";
    FileWrite::WriteFromClass(4, data);
#endif
}

void BluetoothDiscovery::discoveryDevice_stoped()
{
    discoveryDeviceAgent->stop();
    qDebug() << "Cканирование устройств остановлено";
    data = "Cканирование устройств остановлено";
    FileWrite::WriteFromClass(4, data);
}

void BluetoothDiscovery::discoveryService_stoped()
{
    discoveryServiceAgent->stop();
    qDebug() << "Cканирование сервисов остановлено";
    data = "Cканирование сервисов остановлено";
    FileWrite::WriteFromClass(4, data);
}

#ifndef BLUETOOTHDISCOVERY_H
#define BLUETOOTHDISCOVERY_H
#include <QBluetoothLocalDevice>
#include <QBluetoothServiceDiscoveryAgent>
#include <QBluetoothServiceInfo>
#include <QBluetoothUuid>
#include <QThread>

#include <model/model.h>
#include <streams/filewrite.h>
#include <streams/timerrssi.h>

class BluetoothDiscovery: public QObject
{
    Q_OBJECT
public:
    BluetoothDiscovery(QObject *parent = nullptr);

signals:
    void deviceFound(QBluetoothServiceInfo);
    void doTimer(int);

public slots:
    void SetHostDiscoverable();
    void StartDeviceDiscovery();
    void StartServiceDiscovery();

private slots:
    void UpdateRSSI();

private:
    QBluetoothServiceDiscoveryAgent *discoveryServiceAgent = new QBluetoothServiceDiscoveryAgent;
    QBluetoothDeviceDiscoveryAgent *discoveryDeviceAgent = new QBluetoothDeviceDiscoveryAgent;
    QBluetoothLocalDevice *localDevice = new QBluetoothLocalDevice(this);

    Model *md;
    FileWrite *fw;
    QString data;

};

#endif // BLUETOOTHDISCOVERY_H

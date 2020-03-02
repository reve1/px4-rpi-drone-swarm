#ifndef VEHICLE_H
#define VEHICLE_H

#include <QObject>
#include <QDebug>

#include <mavsdk/include/mavsdk/mavsdk.h>
#include <mavsdk/include/mavsdk/plugins/action/action.h>
#include <mavsdk/include/mavsdk/plugins/telemetry/telemetry.h>
#include <streams/filewrite.h>
#include <model/model.h>

class Vehicle : public QObject
{
    Q_OBJECT

public:
    Vehicle();

private:
    FileWrite *fw;
    Model *md;
};

#endif // VEHICLE_H

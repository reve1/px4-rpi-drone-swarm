#ifndef VEHICLE_H
#define VEHICLE_H

#include <QObject>
#include <QDebug>
#include <QtConcurrent>

#include <mavsdk/include/mavsdk/mavsdk.h>
#include <mavsdk/include/mavsdk/plugins/action/action.h>
#include <mavsdk/include/mavsdk/plugins/telemetry/telemetry.h>
#include <mavsdk/include/mavsdk/plugins/follow_me/follow_me.h>
#include <mavsdk/geometry.h>
#include <streams/filewrite.h>
#include <model/model.h>
#include <iostream>

using namespace std::this_thread;
using namespace std::chrono;

class Vehicle : public QObject
{
    Q_OBJECT

public:
    Vehicle();
signals:
    void LocalVehiclePositionInfo(unsigned long,double,double,float,float);
    void LocalVehicleGPSInfo(unsigned long,int,int);
    void LocalVehicleBatteryInfo(unsigned long,float);
    void LocalVehicleAngle(unsigned long,float);
    void LocalVehicleFlightMode(unsigned long,int);
    void newCoordSet();
public slots:
    void Run();
    void fly(const double &LAT,
             const double &LON,
             const float &AMSL,
             const float &angle_yaw);
    void setReturnToLaunch();
    void setLand();
    void setTakeOff();

private:
    void getTelemetry(std::shared_ptr<mavsdk::Telemetry> telemetry);
    void setTelemetryRate(std::shared_ptr<mavsdk::Telemetry> telemetry);
    void setArm();

    void setGoToLocation(std::shared_ptr<mavsdk::Action> action);
    QString data;
    mavsdk::Telemetry::FlightMode oldFlightMode = mavsdk::Telemetry::FlightMode::Unknown;
    mavsdk::Telemetry::LandedState oldLandedState = mavsdk::Telemetry::LandedState::Unknown;
    mavsdk::ConnectionResult connection_result;
    mavsdk::Mavsdk dc;
    mavsdk::System &system = dc.system();
};

#endif // VEHICLE_H

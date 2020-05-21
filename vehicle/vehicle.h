#ifndef VEHICLE_H
#define VEHICLE_H

#include <QObject>
#include <QDebug>
#include <QtConcurrent>

#include <mavsdk/include/mavsdk/mavsdk.h>
#include <mavsdk/include/mavsdk/plugins/action/action.h>
#include <mavsdk/include/mavsdk/plugins/telemetry/telemetry.h>
#include <mavsdk/include/mavsdk/plugins/follow_me/follow_me.h>
#include <streams/filewrite.h>
#include <model/model.h>


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
    void newCoordSet();
public slots:
    void Run();
    void fly(const double &LAT,
             const double &LON,
             const float &AMSL,
             const float &angle_yaw);


private:
    void getTelemetry(std::shared_ptr<mavsdk::Telemetry> telemetry);
    void setTelemetryRate(std::shared_ptr<mavsdk::Telemetry> telemetry);
    void setArm(std::shared_ptr<mavsdk::Action> action);
    void setLand(std::shared_ptr<mavsdk::Action> action);
    void setTakeOff(std::shared_ptr<mavsdk::Action> action);
    void setGoToLocation(std::shared_ptr<mavsdk::Action> action);



    QString data;
    mavsdk::ConnectionResult connection_result;
    mavsdk::Mavsdk dc;

    mavsdk::System &system = dc.system();
};

#endif // VEHICLE_H

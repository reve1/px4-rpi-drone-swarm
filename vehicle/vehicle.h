#ifndef VEHICLE_H
#define VEHICLE_H

#include <QObject>
#include <QDebug>

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
    void LocalVehicleInfo(unsigned long,double,double,float,float,int,int);
    void newCoordSet();
public slots:
    void Run();
    void folowMeSetCoord(const double &coord_lat,const double &coord_lon);
    void folowMeStart();

private:
    void getTelemetryAlt(std::shared_ptr<mavsdk::Telemetry> telemetry);
    void setTelemetryRate(std::shared_ptr<mavsdk::Telemetry> telemetry);

    void setArm(std::shared_ptr<mavsdk::Action> action);
    void setLand(std::shared_ptr<mavsdk::Action> action);
    void setTakeOff(std::shared_ptr<mavsdk::Action> action);
    void setGoToLocation(std::shared_ptr<mavsdk::Action> action);

    void folowMeStop(std::shared_ptr<mavsdk::FollowMe> follow_me);

mavsdk::FollowMe::TargetLocation target_location{};

QString data;
mavsdk::Mavsdk dc;
mavsdk::ConnectionResult connection_result;
mavsdk::System &system = dc.system();

private slots:
    void folowMeSetTarget(std::shared_ptr<mavsdk::FollowMe> follow_me);

    //


};

#endif // VEHICLE_H

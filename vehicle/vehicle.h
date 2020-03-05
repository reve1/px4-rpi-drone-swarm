#ifndef VEHICLE_H
#define VEHICLE_H

#include <QObject>
#include <QDebug>

#include <mavsdk/include/mavsdk/mavsdk.h>
#include <mavsdk/include/mavsdk/plugins/action/action.h>
#include <mavsdk/include/mavsdk/plugins/telemetry/telemetry.h>
#include <streams/filewrite.h>
#include <model/model.h>

using namespace std::this_thread;
using namespace std::chrono;

class Vehicle : public QObject
{
    Q_OBJECT

public:
    Vehicle();

private:
    void getTelemetryAlt(std::shared_ptr<mavsdk::Telemetry> telemetry);
    void setTelemetryRate(std::shared_ptr<mavsdk::Telemetry> telemetry);
    void setTakeOff(std::shared_ptr<mavsdk::Action> action);
    void setArm(std::shared_ptr<mavsdk::Action> action);
    void setLand(std::shared_ptr<mavsdk::Action> action);

    FileWrite *fw;
    QString data;
    mavsdk::Mavsdk dc;
    mavsdk::ConnectionResult connection_result;
    mavsdk::System &system = dc.system();
};

#endif // VEHICLE_H

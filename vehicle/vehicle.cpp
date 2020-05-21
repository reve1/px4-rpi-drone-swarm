#include "vehicle.h"

Vehicle::Vehicle()
{
    //connect(this, &Vehicle::newCoordSet, this, &Vehicle::folowMeSetTarget);
    bool discovered_system = false;
    connection_result = dc.add_udp_connection( "localhost", 14540); // MAV_1
    //connection_result = dc.add_udp_connection( "localhost", 14541); // MAV_2
    //connection_result = dc.add_serial_connection("/dev/ttyS0", 57600);
    //connection_result = dc.add_serial_connection("/dev/ttyUSB0", 57600);

    if (connection_result != mavsdk::ConnectionResult::SUCCESS) {
        qDebug() << "Результат подключения: " << connection_result_str(connection_result);
        //return 1;
    }

    qDebug() << "Ищем систему...";

    dc.register_on_discover([&discovered_system](uint64_t uuid) {
        qDebug() << "Найдена система: " << uuid;
        discovered_system = true;
    });

    auto telemetry = std::make_shared<mavsdk::Telemetry>(system);
    auto action = std::make_shared<mavsdk::Action>(system);

    while (telemetry->health_all_ok() != true) {
        qDebug() << "БВС не готов к запуску";
        sleep_for(seconds(1));
    }

    setTelemetryRate(telemetry);
    getTelemetry(telemetry);
}

void Vehicle::Run()
{

    sleep_for(seconds(100));
    //setArm(action);
    //setTakeOff(action);
    //getTelemetry(telemetry);
    double targetLat = 44.0769288; //x
    double targetLon = 43.0879335; //y

    //fly(targetLat,targetLon,540,0);
    //setGoToLocation(action);


    //sleep_for(seconds(120));
    //getTelemetry(telemetry);
    //setLand(action);
    //sleep_for(seconds(2));
    //getTelemetry(telemetry);
    //action->return_to_launch();
}

void Vehicle::setTelemetryRate(std::shared_ptr<mavsdk::Telemetry> telemetry)
{
    const mavsdk::Telemetry::Result set_rate_result = telemetry->set_rate_position(1.0);
    if (set_rate_result != mavsdk::Telemetry::Result::SUCCESS) {
        qDebug() << "Ошибка установки телеметрии";
        data = "Ошибка установки телеметрии";
        FileWrite::WriteFromClass(5, data.simplified());
    }
    while (telemetry->health_all_ok() != true) {
        qDebug() << "БВС не готов к запуску";
        data = "БВС не готов к запуску";
        FileWrite::WriteFromClass(5, data.simplified());
        sleep_for(seconds(1));
    }
    qDebug() << "Отправлен сигнал установки телеметрии";
    data = "Отправлен сигнал установки телеметрии";
    FileWrite::WriteFromClass(5, data.simplified());
}

void Vehicle::setArm(std::shared_ptr<mavsdk::Action> action)
{
    mavsdk::Action::Result arm = action->arm();
    if (arm != mavsdk::Action::Result::SUCCESS) {
        qDebug() << "Ошибка арминга";
        data = "Ошибка арминга";
        FileWrite::WriteFromClass(5, data.simplified());
        return;
    }
    qDebug() << "Отправлен сигнал арминга";
    data = "Отправлен сигнал арминга";
    FileWrite::WriteFromClass(5, data.simplified());
}

void Vehicle::setTakeOff(std::shared_ptr<mavsdk::Action> action)
{
    mavsdk::Action::Result takeoff = action->takeoff();
    if (takeoff != mavsdk::Action::Result::SUCCESS) {
        qDebug() << "Ошибка взлета";
        data = "Ошибка взлета";
        FileWrite::WriteFromClass(5, data.simplified());
        return;
    }
    qDebug() << "Отправлен сигнал взлета";
    data = "Отправлен сигнал взлета";
    FileWrite::WriteFromClass(5, data.simplified());
    sleep_for(seconds(20));
}

void Vehicle::setLand(std::shared_ptr<mavsdk::Action> action)
{
    mavsdk::Action::Result land = action->land();
    if (land != mavsdk::Action::Result::SUCCESS) {
        qDebug() << "Ошибка посадки";
        data = "Ошибка посадки";
        FileWrite::WriteFromClass(5, data.simplified());
        return;
    }
    qDebug() << "Отправлен сигнал посадки";
    data = "Отправлен сигнал посадки";
    FileWrite::WriteFromClass(5, data.simplified());
}

void Vehicle::setGoToLocation(std::shared_ptr<mavsdk::Action> action)
{
    mavsdk::Action::Result goto_location_result = action->goto_location(44.0769288,43.0879335,540,0);
    if (goto_location_result != mavsdk::Action::Result::SUCCESS){
        qDebug() << "Ошибка движения БВС к заданной точке";
        data = "Ошибка движения БВС к заданной точке";
        FileWrite::WriteFromClass(5, data.simplified());
    }
    qDebug() << "Отправлен сигнал движения БВС к заданной точке";
    data = "Отправлен сигнал движения БВС к заданной точке";
    FileWrite::WriteFromClass(5, data.simplified());
}

void Vehicle::getTelemetry(std::shared_ptr<mavsdk::Telemetry> telemetry)
{
    telemetry->position_async([this](mavsdk::Telemetry::Position position)
    {
        unsigned long UUID = system.get_uuid();
        double LON = position.longitude_deg;
        double LAT = position.latitude_deg;
        float ALT = position.relative_altitude_m;
        float AMSL = position.absolute_altitude_m;

        //qDebug() << "Высота: " << ALT << " m";
        //data = "Высота: " + QString::number(ALT) + " m";
        //FileWrite::WriteFromClass(5, data.simplified());
        //qDebug() <<"Широта: " << LAT;
        //data = "Широта: " + QString::number(LAT);
        //FileWrite::WriteFromClass(5, data.simplified());
        //qDebug() <<"Долгота: " << LON;
        //data = "Долгота: " + QString::number(LON);
        //FileWrite::WriteFromClass(5, data.simplified());
        //qDebug() <<"Высота AMSL: " << AMSL << " m";
        //data = "Высота AMSL: " + QString::number(AMSL) + " m";
        //FileWrite::WriteFromClass(5, data.simplified());

        emit LocalVehiclePositionInfo(UUID,LAT,LON,ALT,AMSL);
    });

    telemetry->gps_info_async([this](mavsdk::Telemetry::GPSInfo gpsinfo)
    {
        unsigned long UUID = system.get_uuid();
        int GPS_num = gpsinfo.num_satellites;
        int GPS_fix_type = gpsinfo.fix_type;
        //qDebug() <<"Спутников GPS: " << GPS;
        //data = &"Спутников GPS: " [GPS_num];
        //FileWrite::WriteFromClass(5, data.simplified());
        //qDebug() <<"Статус GPS: " << GPS_fix_type;
        //data = &"Статус GPS: " [GPS_fix_type];
        emit LocalVehicleGPSInfo(UUID,GPS_num,GPS_fix_type);
    });

    telemetry->battery_async([this](mavsdk::Telemetry::Battery battery)
    {
        unsigned long UUID = system.get_uuid();
        float battery_remaining_percent = battery.remaining_percent;
        //data = "Оставшийся заряд батареи: " + QString::number(battery_remaining_percent);
        //FileWrite::WriteFromClass(5, data.simplified());
        //qDebug() <<"Оставшийся заряд батареи: " << battery.remaining_percent;
        emit LocalVehicleBatteryInfo(UUID,battery_remaining_percent);
    });

    telemetry->attitude_euler_angle_async([this](mavsdk::Telemetry::EulerAngle euler_angle)
    {
        unsigned long UUID = system.get_uuid();
        float angle_yaw = euler_angle.yaw_deg;
        //data = "Угол отклонения: " + QString::number(angle_yaw);
        //FileWrite::WriteFromClass(5, data.simplified());
        //qDebug() <<"Угол отклонения: " << angle_yaw;
        emit LocalVehicleAngle(UUID,angle_yaw);
    });
}

void Vehicle::fly(const double &LAT, const double &LON, const float &AMSL, const float &angle_yaw)
{
    auto action = std::make_shared<mavsdk::Action>(system);
    action->goto_location(LAT,LON,AMSL,angle_yaw);
}


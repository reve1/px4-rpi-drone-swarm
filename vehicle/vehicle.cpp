#include "vehicle.h"

Vehicle::Vehicle()
{
    bool discovered_system = false;
    connection_result = dc.add_udp_connection( "localhost", 14550);
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
    getTelemetryAlt(telemetry);
    setArm(action);
    setTakeOff(action);
    getTelemetryAlt(telemetry);
    sleep_for(seconds(20));
    const mavsdk::Action::Result goto_location_result = action->goto_location(47.397,8.54559,507.502,0);
    if (goto_location_result != mavsdk::Action::Result::SUCCESS){
        qDebug() << "Ошибка движения БВС к заданной точке";
        data = "Ошибка движения БВС к заданной точке";
        fw->WriteFromClass(5, data.simplified());
    }
    sleep_for(seconds(120));
    getTelemetryAlt(telemetry);
    setLand(action);
    sleep_for(seconds(2));
    getTelemetryAlt(telemetry);
    //action->return_to_launch();
}

void Vehicle::setTelemetryRate(std::shared_ptr<mavsdk::Telemetry> telemetry)
{
    const mavsdk::Telemetry::Result set_rate_result = telemetry->set_rate_position(1.0);
    if (set_rate_result != mavsdk::Telemetry::Result::SUCCESS) {
        qDebug() << "Ошибка установки телеметрии";
        data = "Ошибка установки телеметрии";
        fw->WriteFromClass(5, data.simplified());
    }
    while (telemetry->health_all_ok() != true) {
        qDebug() << "БВС не готов к запуску";
        data = "БВС не готов к запуску";
        fw->WriteFromClass(5, data.simplified());
        sleep_for(seconds(1));
    }
}

void Vehicle::setArm(std::shared_ptr<mavsdk::Action> action)
{
    mavsdk::Action::Result arm = action->arm();
    if (arm != mavsdk::Action::Result::SUCCESS) {
        qDebug() << "Ошибка арминга";
        data = "Ошибка арминга";
        fw->WriteFromClass(5, data.simplified());
        return;
    }
}

void Vehicle::setTakeOff(std::shared_ptr<mavsdk::Action> action)
{
    mavsdk::Action::Result takeoff = action->takeoff();
    if (takeoff != mavsdk::Action::Result::SUCCESS) {
        qDebug() << "Ошибка взлета";
        data = "Ошибка взлета";
        fw->WriteFromClass(5, data.simplified());
        return;
    }
}

void Vehicle::setLand(std::shared_ptr<mavsdk::Action> action)
{
    mavsdk::Action::Result land = action->land();
    if (land != mavsdk::Action::Result::SUCCESS) {
        qDebug() << "Ошибка посадки";
        data = "Ошибка посадки";
        fw->WriteFromClass(5, data.simplified());
        return;
    }
}

void Vehicle::getTelemetryAlt(std::shared_ptr<mavsdk::Telemetry> telemetry)
{
    qDebug() <<"Высота: " << telemetry->position().relative_altitude_m << " m";
    data = "Высота: " + QString::number(telemetry->position().relative_altitude_m) + " m";
    fw->WriteFromClass(5, data.simplified());
    qDebug() <<"Широта: " << telemetry->position().latitude_deg;
    data = "Широта: " + QString::number(telemetry->position().latitude_deg);
    fw->WriteFromClass(5, data.simplified());
    qDebug() <<"Долгота: " << telemetry->position().longitude_deg;
    data = "Долгота: " + QString::number(telemetry->position().longitude_deg);
    fw->WriteFromClass(5, data.simplified());
    qDebug() <<"Высота AMSL: " << telemetry->position().absolute_altitude_m << " m";
    data = "Высота AMSL: " + QString::number(telemetry->position().absolute_altitude_m) + " m";
    fw->WriteFromClass(5, data.simplified());
    qDebug() <<"Спутников GPS: " << telemetry->gps_info().num_satellites;
    data = &"Спутников GPS: " [telemetry->gps_info().num_satellites];
    fw->WriteFromClass(5, data.simplified());
    qDebug() <<"Статус GPS: " << telemetry->gps_info().fix_type;
    data = &"Статус GPS: " [telemetry->gps_info().fix_type];
    fw->WriteFromClass(5, data.simplified());
}



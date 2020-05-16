#include "vehicle.h"

Vehicle::Vehicle()
{
}
void Vehicle::Run()
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
    setGoToLocation(action);
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

void Vehicle::getTelemetryAlt(std::shared_ptr<mavsdk::Telemetry> telemetry)
{
    unsigned long UUID = system.get_uuid();
    double LON = telemetry->position().longitude_deg;
    double LAT = telemetry->position().latitude_deg;
    float ALT = telemetry->position().relative_altitude_m;
    float AMSL = telemetry->position().absolute_altitude_m;
    int GPS = telemetry->gps_info().num_satellites;
    int GPS_fix_type = telemetry->gps_info().fix_type;
    //telemetry->battery().remaining_percent;

    qDebug() << "Высота: " << ALT << " m";
    data = "Высота: " + QString::number(ALT) + " m";
    FileWrite::WriteFromClass(5, data.simplified());
    qDebug() <<"Широта: " << LAT;
    data = "Широта: " + QString::number(LAT);
    FileWrite::WriteFromClass(5, data.simplified());
    qDebug() <<"Долгота: " << LON;
    data = "Долгота: " + QString::number(LON);
    FileWrite::WriteFromClass(5, data.simplified());
    qDebug() <<"Высота AMSL: " << AMSL << " m";
    data = "Высота AMSL: " + QString::number(AMSL) + " m";
    FileWrite::WriteFromClass(5, data.simplified());
    qDebug() <<"Спутников GPS: " << GPS;
    data = &"Спутников GPS: " [GPS];
    FileWrite::WriteFromClass(5, data.simplified());
    qDebug() <<"Статус GPS: " << GPS_fix_type;
    data = &"Статус GPS: " [GPS_fix_type];
    FileWrite::WriteFromClass(5, data.simplified());
    qDebug() <<"Статус GPS: " << telemetry->battery().remaining_percent;

    emit LocalVehicleInfo(UUID,LAT,LON,ALT,AMSL,GPS,GPS_fix_type);
}



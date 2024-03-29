#include "vehicle.h"

Vehicle::Vehicle()
{

}

void Vehicle::Run()
{
    bool discovered_system = false;

    //connection_result = dc.add_udp_connection( "localhost", 14540); // MAV_1
    //connection_result = dc.add_udp_connection( "localhost", 14541); // MAV_2
    //connection_result = dc.add_udp_connection( "localhost", 14542); // MAV_3
    //connection_result = dc.add_udp_connection( "localhost", 14543); // MAV_4
    //connection_result = dc.add_udp_connection( "localhost", 14544); // MAV_5
    //connection_result = dc.add_udp_connection( "localhost", 14545); // MAV_6
    connection_result = dc.add_udp_connection( "localhost", 14546); // MAV_7

    //connection_result = dc.add_serial_connection("/dev/ttyS0", 57600);
    //connection_result = dc.add_serial_connection("/dev/ttyUSB0", 57600);

    if (connection_result != mavsdk::ConnectionResult::Success) {
        //qDebug() << "Результат подключения: " << connection_result;
        //return 1;
    }

    qDebug() << "Ищем систему...";

    dc.register_on_discover([&discovered_system](uint64_t uuid) {
        qDebug() << "Найдена система: " << uuid;
        discovered_system = true;
    });

    auto telemetry = std::make_shared<mavsdk::Telemetry>(system);

    while (telemetry->health_all_ok() != true) {
        qDebug() << "БВС не готов к запуску";
        sleep_for(seconds(1));
    }

    setTelemetryRate(telemetry);
    getTelemetry(telemetry);

    setArm();
    setTakeOff();

    getTelemetry(telemetry);
    sleep_for(seconds(12000));
}

void Vehicle::setTelemetryRate(std::shared_ptr<mavsdk::Telemetry> telemetry)
{
    const mavsdk::Telemetry::Result set_rate_result = telemetry->set_rate_position(1.0);
    if (set_rate_result != mavsdk::Telemetry::Result::Success) {
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
    emit LocalUUID(system.get_uuid());
    qDebug() << "Отправлен сигнал установки телеметрии";
    data = "Отправлен сигнал установки телеметрии";
    FileWrite::WriteFromClass(5, data.simplified());
}

void Vehicle::setReturnToLaunch()
{
    auto action = std::make_shared<mavsdk::Action>(system);
    mavsdk::Action::Result arm = action->return_to_launch();
    if (arm != mavsdk::Action::Result::Success) {
        qDebug() << "Ошибка возврата";
        data = "Ошибка возврата";
        FileWrite::WriteFromClass(5, data.simplified());
        return;
    }
    qDebug() << "Отправлен сигнал возврата";
    data = "Отправлен сигнал возврата";
    FileWrite::WriteFromClass(5, data.simplified());
}

void Vehicle::setArm()
{
    auto action = std::make_shared<mavsdk::Action>(system);
    mavsdk::Action::Result arm = action->arm();
    if (arm != mavsdk::Action::Result::Success) {
        qDebug() << "Ошибка арминга";
        data = "Ошибка арминга";
        FileWrite::WriteFromClass(5, data.simplified());
        return;
    }
    qDebug() << "Отправлен сигнал арминга";
    data = "Отправлен сигнал арминга";
    FileWrite::WriteFromClass(5, data.simplified());
}

void Vehicle::setTakeOff()
{
    auto action = std::make_shared<mavsdk::Action>(system);
    mavsdk::Action::Result takeoff = action->takeoff();
    if (takeoff != mavsdk::Action::Result::Success) {
        qDebug() << "Ошибка взлета";
        data = "Ошибка взлета";
        FileWrite::WriteFromClass(5, data.simplified());
        return;
    }
    qDebug() << "Отправлен сигнал взлета";
    data = "Отправлен сигнал взлета";
    FileWrite::WriteFromClass(5, data.simplified());
    sleep_for(seconds(20));

    action->set_maximum_speed(3);
}

void Vehicle::setLand()
{
    auto action = std::make_shared<mavsdk::Action>(system);
    mavsdk::Action::Result land = action->land();
    if (land != mavsdk::Action::Result::Success) {
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
    if (goto_location_result != mavsdk::Action::Result::Success){
        qDebug() << "Ошибка движения БВС к заданной точке";
        data = "Ошибка движения БВС к заданной точке";
        FileWrite::WriteFromClass(5, data.simplified());
    }
    qDebug() << "Отправлен сигнал движения БВС к заданной точке";
    data = "Отправлен сигнал движения БВС к заданной точке";    float speed = action->get_maximum_speed().second;
    qDebug () << speed;

    action->set_maximum_speed(3);
    float speed1 = action->get_maximum_speed().second;
    qDebug () << speed1;
    FileWrite::WriteFromClass(5, data.simplified());
}

void Vehicle::getTelemetry(std::shared_ptr<mavsdk::Telemetry> telemetry)
{        
    telemetry->subscribe_position([this](mavsdk::Telemetry::Position position)
    {
        unsigned long UUID = system.get_uuid();
        double LON = position.longitude_deg;
        double LAT = position.latitude_deg;
        float ALT = position.relative_altitude_m;
        float AMSL = position.absolute_altitude_m;

        //qDebug() << "Высота: " << ALT << " m";
        //data = "Высота: " + QString::number(ALT) + " m";
        //FileWrite::WriteFromClass(5, data.simplified());
        //qDebug() << "Широта: " << LAT;
        //qDebug() << "Широта: " << position.latitude_deg;
        //data = "Широта: " + QString::number(LAT);
        //FileWrite::WriteFromClass(5, data.simplified());
        //data = "Долгота: " + QString::number(LON);
        //FileWrite::WriteFromClass(5, data.simplified());
        //qDebug() <<"Высота AMSL: " << AMSL << " m";
        //data = "Высота AMSL: " + QString::number(AMSL) + " m";
        //FileWrite::WriteFromClass(5, data.simplified());

        emit LocalVehiclePositionInfo(UUID,LAT,LON,ALT,AMSL);
    });

    telemetry->subscribe_gps_info([this](mavsdk::Telemetry::GpsInfo gpsinfo)
    {
        unsigned long UUID = system.get_uuid();
        int GPS_num = gpsinfo.num_satellites;
        int GPS_fix_type = 0;
        //qDebug() <<"Спутников GPS: " << GPS;
        //data = &"Спутников GPS: " [GPS_num];
        //FileWrite::WriteFromClass(5, data.simplified());
        //qDebug() <<"Статус GPS: " << GPS_fix_type;
        //data = &"Статус GPS: " [GPS_fix_type];
        emit LocalVehicleGPSInfo(UUID,GPS_num,GPS_fix_type);
    });

    telemetry->subscribe_battery([this](mavsdk::Telemetry::Battery battery)
    {
        unsigned long UUID = system.get_uuid();
        float battery_remaining_percent = battery.remaining_percent;
        //data = "Оставшийся заряд батареи: " + QString::number(battery_remaining_percent);
        //FileWrite::WriteFromClass(5, data.simplified());
        //qDebug() <<"Оставшийся заряд батареи: " << battery.remaining_percent;
        emit LocalVehicleBatteryInfo(UUID,battery_remaining_percent);
    });

    telemetry->subscribe_attitude_euler([this](mavsdk::Telemetry::EulerAngle euler_angle)
    {
        unsigned long UUID = system.get_uuid();
        float angle_yaw = euler_angle.yaw_deg;
        //data = "Угол отклонения: " + QString::number(angle_yaw);
        //FileWrite::WriteFromClass(5, data.simplified());
        //qDebug() <<"Угол отклонения: " << angle_yaw;
        emit LocalVehicleAngle(UUID,angle_yaw);
    });

    telemetry->subscribe_flight_mode([this](mavsdk::Telemetry::FlightMode flightMode) {
        unsigned long UUID = system.get_uuid();
        if (oldFlightMode != flightMode) {
            oldFlightMode = flightMode;
            switch(flightMode)
            {
            case mavsdk::Telemetry::FlightMode::Unknown:
                qDebug() << "Изменен полетный режим, новый режим - Неизвестен.";
                emit LocalVehicleFlightMode(UUID,0);
                break;
            case mavsdk::Telemetry::FlightMode::Takeoff:
                qDebug() << "Изменен полетный режим, новый режим - Взлет.";
                emit LocalVehicleFlightMode(UUID,1);
                break;
            case mavsdk::Telemetry::FlightMode::ReturnToLaunch:
                qDebug() << "Изменен полетный режим, новый режим - Возврат в точку взлета.";
                emit LocalVehicleFlightMode(UUID,2);
                break;
            case mavsdk::Telemetry::FlightMode::Land:
                qDebug() << "Изменен полетный режим, новый режим - Посадка.";
                emit LocalVehicleFlightMode(UUID,3);
                break;
            case mavsdk::Telemetry::FlightMode::Ready:
                qDebug() << "Изменен полетный режим, новый режим - Готовность.";
                emit LocalVehicleFlightMode(UUID,4);
                break;
            default:
                qDebug() << "Изменен полетный режим, новый режим - Полетный.";
                emit LocalVehicleFlightMode(UUID,5);
                break;
            }
        }
    });

    telemetry->subscribe_landed_state([this](mavsdk::Telemetry::LandedState landed_state) {
        unsigned long UUID = system.get_uuid();
        if (oldLandedState != landed_state) {
            oldLandedState = landed_state;
            switch(landed_state)
            {
            case mavsdk::Telemetry::LandedState::Unknown:
                qDebug() << "Изменен полетный режим, новый режим - Неизвестен.";
                emit LocalVehicleFlightMode(UUID,0);
                break;
            case mavsdk::Telemetry::LandedState::OnGround:
                qDebug() << "Изменен полетный режим, новый режим - На земле.";
                emit LocalVehicleFlightMode(UUID,6);
                break;
            case mavsdk::Telemetry::LandedState::InAir:
                qDebug() << "Изменен полетный режим, новый режим - В воздухе.";
                emit LocalVehicleFlightMode(UUID,5);
                break;
            case mavsdk::Telemetry::LandedState::TakingOff:
                qDebug() << "Изменен полетный режим, новый режим - Взлет.";
                emit LocalVehicleFlightMode(UUID,1);
                break;
            case mavsdk::Telemetry::LandedState::Landing:
                qDebug() << "Изменен полетный режим, новый режим - Посадка.";
                emit LocalVehicleFlightMode(UUID,3);
                break;
            }
        }
    });
}

void Vehicle::fly(const double &LAT, const double &LON, const float &AMSL, const float &angle_yaw)
{
    auto action = std::make_shared<mavsdk::Action>(system);
    action->goto_location_async(LAT,LON,AMSL,angle_yaw,nullptr);

    action->set_maximum_speed(5);
}

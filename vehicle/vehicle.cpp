#include "vehicle.h"

Vehicle::Vehicle()
{
    QString data;
    mavsdk::Mavsdk dc;
    mavsdk::ConnectionResult connection_result;
    bool discovered_system = false;
    //connection_result = dc.add_serial_connection("/dev/ttyS0", 57600);
    //connection_result = dc.add_serial_connection("/dev/ttyUSB0", 57600);
    connection_result = dc.add_udp_connection( "localhost", 14550);

    if (connection_result != mavsdk::ConnectionResult::SUCCESS)
    {
        qDebug() << "Результат подключения: " << connection_result_str(connection_result);
        data = "Результат подключения: УСПЕХ";
        fw->WriteFromClass(5, data.simplified());
        //return 1;
    }

    mavsdk::System &system = dc.system();
    dc.register_on_discover([&discovered_system](uint64_t uuid)
    {

        qDebug() << "Найдена система: " << uuid;
        discovered_system = true;
        QString data;
        FileWrite *fw;
        data = "Найдена система: " + uuid;
        fw->WriteFromClass(5, data.simplified());
        qDebug() << data;

    });

    //system.register_component_discovered_callback(component_discovered);

    auto action = std::make_shared<mavsdk::Action>(system);
    const mavsdk::Action::Result arm_result = action->arm();
    if (arm_result != mavsdk::Action::Result::SUCCESS){
        qDebug() << "Ошибка запуска БВС.";
        data = "Ошибка запуска БВС.";
        fw->WriteFromClass(5, data.simplified());
    }
    const mavsdk::Action::Result takeoff_result = action->takeoff();
    action->set_takeoff_altitude(10);
    if (takeoff_result != mavsdk::Action::Result::SUCCESS){
        qDebug() << "Ошибка набора высоты БВС.";
        data = "Ошибка набора высоты БВС.";
        fw->WriteFromClass(5, data.simplified());
    }

    const mavsdk::Action::Result goto_location_result = action->goto_location(47.397,8.54559,507.502,0);
    if (goto_location_result != mavsdk::Action::Result::SUCCESS){
        qDebug() << "Ошибка движения БВС к заданной точке";
        data = "Ошибка движения БВС к заданной точке";
        fw->WriteFromClass(5, data.simplified());
    }
    //action->return_to_launch();

    auto telemetry = std::make_shared<mavsdk::Telemetry>(system);
    const mavsdk::Telemetry::Result set_rate_result = telemetry->set_rate_position(1.0);
    if (set_rate_result != mavsdk::Telemetry::Result::SUCCESS) {
        qDebug() << "Ошибка задания частоты телеметрии БВС: " << mavsdk::Telemetry::result_str(set_rate_result);
        data = "Ошибка задания частоты телеметрии БВС";
        fw->WriteFromClass(5, data.simplified());
    }

    telemetry->gps_info_async([](mavsdk::Telemetry::GPSInfo info) {
        qDebug() << "Найдено спутников GPS: " << info.num_satellites;
        qDebug() <<"Координаты БВС: " << info.fix_type;
    });

    // Set up callback to monitor altitude while the vehicle is in flight
    telemetry->position_async([](mavsdk::Telemetry::Position position) {
        Model *md;
        qDebug() <<"Altitude: " << position.relative_altitude_m << " m";
        qDebug() <<"Latitude: " << position.latitude_deg;
        qDebug() <<"Longitude: " << position.longitude_deg;
        qDebug() <<"Altitude AMSL: " << position.absolute_altitude_m << " m";
        //md->setGlobalPositionLat(position.relative_altitude_m);
    });


    // Check if vehicle is ready to arm
    //while (telemetry->health_all_ok() != true) {
    //    qDebug() <<"Vehicle is getting ready to arm";
    //}
    //=====================================================================//
}

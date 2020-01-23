#include "vehicle.h"

Vehicle::Vehicle()
{
    //=====================================================================//
    mavsdk::Mavsdk dc;
    mavsdk::ConnectionResult connection_result;
    bool discovered_system = false;
    //connection_result = dc.add_serial_connection("/dev/ttyS0", 57600);
    connection_result = dc.add_udp_connection( "localhost", 14550);

    if (connection_result != mavsdk::ConnectionResult::SUCCESS)
    {
        qDebug() << "Результат подключения: " << connection_result_str(connection_result);
        //return 1;
    }

    mavsdk::System &system = dc.system();
    dc.register_on_discover([&discovered_system](uint64_t uuid)
    {
        qDebug() << "Найдена система: " << uuid;
        discovered_system = true;
    });

    //system.register_component_discovered_callback(component_discovered);
    //mavsdk::Action::Action(mavsdk::System &system);
    //auto action = std::make_shared<mavsdk::Action>(system);

    auto action = std::make_shared<mavsdk::Action>(system);
    const mavsdk::Action::Result arm_result = action->arm();
    const mavsdk::Action::Result takeoff_result = action->takeoff();
    //action->goto_location(1,1,1,1);
    //mavsdk::Action::takeoff() 15;
    
    auto telemetry = std::make_shared<mavsdk::Telemetry>(system);
    const mavsdk::Telemetry::Result set_rate_result = telemetry->set_rate_position(1.0);
    if (set_rate_result != mavsdk::Telemetry::Result::SUCCESS) {
        qDebug() << "Setting rate failed:" << mavsdk::Telemetry::result_str(set_rate_result);
    }
        qDebug() <<"Altitude1: ";
    // Set up callback to monitor altitude while the vehicle is in flight
    telemetry->position_async([](mavsdk::Telemetry::Position position) {
        qDebug() <<"Altitude: " << position.relative_altitude_m << " m";
    });
            qDebug() <<"Altitude2: ";

    // Check if vehicle is ready to arm
    //while (telemetry->health_all_ok() != true) {
    //    qDebug() <<"Vehicle is getting ready to arm";
    //}
    //=====================================================================//

}

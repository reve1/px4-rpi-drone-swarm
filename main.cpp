/*
sudo hciconfig 0 sspmode 0
sudo rfcomm connect hci0 B8:27:EB:89:C5:44 8
*/

#include <QCoreApplication>
#include <qbluetooth.h>
#include <qbluetoothdevicediscoveryagent.h>
#include <QBluetoothLocalDevice>
#include <QLoggingCategory>

#include <streams/filewrite.h>
#include <streams/timerrssi.h>
#include <streams/timermessage.h>
#include <bluetooth/chat.h>
#include <bluetooth/bluetoothdiscovery.h>

#if !defined (Q_OS_WIN) || (Q_OS_ANDROID)
#include <mavsdk/include/mavsdk/mavsdk.h>
#include <mavsdk/include/mavsdk/plugins/action/action.h>
#include <mavsdk/include/mavsdk/plugins/telemetry/telemetry.h>
#endif

//#include <vehicle/vehicle.h>

#include <model/model.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    //QLoggingCategory::setFilterRules(QStringLiteral("qt.bluetooth* = true"));
    Model *md = new Model;
    FileWrite *fw = new FileWrite;
    Chat *bchat = new Chat;
    //BluetoothDiscovery *bd = new BluetoothDiscovery;

    //QObject::connect(bd, SIGNAL(deviceFound(QBluetoothServiceInfo)), bchat, SLOT(connectClicked(QBluetoothServiceInfo)));
    //QObject::connect(bchat, SIGNAL(socketError()), bd, SLOT(StartServiceDiscovery()));


    //bd->StartServiceDiscovery();

    //Vehicle *vh = new Vehicle;
#if !defined (Q_OS_WIN) || (Q_OS_ANDROID)

    //=====================================================================//
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
        qDebug() <<"Altitude: " << position.relative_altitude_m << " m";
        qDebug() <<"Latitude: " << position.latitude_deg;
        qDebug() <<"Longitude: " << position.longitude_deg;
        qDebug() <<"Altitude AMSL: " << position.absolute_altitude_m << " m";
        //md->Model::ControllerModel::GlobalPositionAlt == position.absolute_altitude_m;
    });

    // Check if vehicle is ready to arm
    //while (telemetry->health_all_ok() != true) {
    //    qDebug() <<"Vehicle is getting ready to arm";
    //}
    //=====================================================================//
#endif
    return a.exec();
}

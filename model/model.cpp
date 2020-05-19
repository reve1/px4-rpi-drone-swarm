#include "model.h"

Model::Model()
{
    connect(&timer, &QTimer::timeout, this, &Model::TimeStampCheck);
    connect(&timer, &QTimer::timeout, this, &Model::sendTimer);
    timer.start(1000);
}

void Model::sendTimer()
{
    emit sendLocalVehicleInfo(local_UUID,
                              VehicleGPLat.value(local_UUID),
                              VehicleGPLon.value(local_UUID),
                              VehicleGPAlt.value(local_UUID),
                              VehicleGPAMSL.value(local_UUID),
                              VehicleGPSStatus.value(local_UUID),
                              VehicleGPS_fix_type.value(local_UUID),
                              VehicleBattery.value(local_UUID));
}

void Model::TimeStampCheck()
{
    foreach (unsigned long key, VehicleTimeStamp.keys())
    {
        QDateTime value=VehicleTimeStamp.value(key);
        if (value.addSecs(5) < QDateTime::currentDateTime())
        {
            VehicleTimeStamp.remove(key);
            VehicleGPAlt.remove(key);
            VehicleGPLat.remove(key);
            VehicleGPLon.remove(key);
            VehicleGPAMSL.remove(key);
            VehicleGPSStatus.remove(key);
            VehicleGPS_fix_type.remove(key);
            VehicleBattery.remove(key);
            VehicleLocalFlag.remove(key);
            qDebug() << "Кеш очищен, удалены устаревшие значение UUID = " << key ;
        }
    }
}

void Model::setLocalVehiclePositionInfo(const unsigned long &UUID,
                                        const double &Lat,
                                        const double &Lon,
                                        const float &Alt,
                                        const float &AMSL)
{
    VehicleGPLat.insert(UUID,Lat);
    VehicleGPLon.insert(UUID,Lon);
    VehicleGPAlt.insert(UUID,Alt);
    VehicleGPAMSL.insert(UUID,AMSL);
    local_UUID = UUID;
    VehicleLocalFlag.insert(UUID,1);
    VehicleTimeStamp.insert(UUID,QDateTime::currentDateTime());
    qDebug() << "Локальное значение";
}

void Model::setLocalVehicleGPSInfo(const unsigned long &UUID,
                                   const int &GPS_num,
                                   const int &GPS_fix_type)
{
    VehicleGPSStatus.insert(UUID,GPS_num);
    VehicleGPS_fix_type.insert(UUID,GPS_fix_type);
}
void Model::setLocalVehicleBatteryInfo(const unsigned long &UUID,
                                       const float &Battery)
{
    VehicleBattery.insert(UUID,Battery);
}

void Model::setRemoteVehicleInfo(const unsigned long &UUID,
                                 const double &Lat,
                                 const double &Lon,
                                 const float &Alt,
                                 const float &AMSL,
                                 const int &GPS_num,
                                 const int &GPS_fix_type,
                                 const float &Battery)
{
    if (VehicleLocalFlag.value(UUID)!= 1)
    {
        VehicleGPAlt.insert(UUID,Alt);
        VehicleGPLat.insert(UUID,Lat);
        VehicleGPLon.insert(UUID,Lon);
        VehicleGPAMSL.insert(UUID,AMSL);
        VehicleGPSStatus.insert(UUID,GPS_num);
        VehicleGPS_fix_type.insert(UUID,GPS_fix_type);
        VehicleBattery.insert(UUID,Battery);
        VehicleTimeStamp.insert(UUID,QDateTime::currentDateTime());
        qDebug() << "Сетевое значение";
        newCoordSet(Lat, Lon);
        return;
    }
}

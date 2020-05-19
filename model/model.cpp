#include "model.h"

Model::Model()
{
    connect(&timer, &QTimer::timeout, this, &Model::TimeStampCheck);
    startTimerTimeStampCheck();
}

void Model::startTimerTimeStampCheck()
{
    timer.start(1000);
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
            VehicleLocalFlag.insert(UUID,1);
            qDebug() << "Кеш очищен, удалены устаревшие значение UUID = " << key ;
        }
    }
}

void Model::setLocalVehiclePositionInfo(const unsigned long &UUID,const double &Lat,const double &Lon,const float &Alt,const float &AMSL)
{
    VehicleGPAlt.insert(UUID,Alt);
    VehicleGPLat.insert(UUID,Lat);
    VehicleGPLon.insert(UUID,Lon);
    VehicleGPAMSL.insert(UUID,AMSL);
    //VehicleGPSStatus.insert(UUID,GPS);
    //VehicleGPS_fix_type.insert(UUID,GPS_fix_type);
    VehicleLocalFlag.insert(UUID,1);
    VehicleTimeStamp.insert(UUID,QDateTime::currentDateTime());
    qDebug() << VehicleGPAlt.value(UUID);
    qDebug() << "Локальное значение";
}

void Model::setRemoteVehicleInfo(const unsigned long &UUID,const double &Lat,const double &Lon,const float &Alt,const float &AMSL,const int &GPS,const int &GPS_fix_type)
{
    if (VehicleLocalFlag.value(UUID)!= 1)
    {
        VehicleGPAlt.insert(UUID,Alt);
        VehicleGPLat.insert(UUID,Lat);
        VehicleGPLon.insert(UUID,Lon);
        VehicleGPAMSL.insert(UUID,AMSL);
        VehicleGPSStatus.insert(UUID,GPS);
        VehicleGPS_fix_type.insert(UUID,GPS_fix_type);
        VehicleTimeStamp.insert(UUID,QDateTime::currentDateTime());
        qDebug() << VehicleGPAlt.value(UUID);
        qDebug() << "Сетевое значение";
        newCoordSet(Lat, Lon);
        return;
    }
}

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
            qDebug() << value << "УДАЛЕН СОВСЕМ ОГО!!!!";
        }
    }
}

void Model::setLocalVehicleInfo(const unsigned long &UUID,const double &Lat,const double &Lon,const float &Alt,const float &AMSL, const int &GPS ,const int &GPS_fix_type)
{
    VehicleGPAlt.insert(UUID,Alt);
    VehicleGPLat.insert(UUID,Lat);
    VehicleGPLon.insert(UUID,Lon);
    VehicleGPAMSL.insert(UUID,AMSL);
    VehicleGPSStatus.insert(UUID,GPS);
    VehicleGPSStatus.insert(UUID,GPS_fix_type);
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
        VehicleGPSStatus.insert(UUID,GPS_fix_type);
        VehicleTimeStamp.insert(UUID,QDateTime::currentDateTime());
        qDebug() << VehicleGPAlt.value(UUID);
        qDebug() << "Сетевое значение";
        return;
    }
}

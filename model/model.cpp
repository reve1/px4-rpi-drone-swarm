#include "model.h"

Model::Model()
{
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
    qDebug() << VehicleGPAlt.value(UUID);
}

void Model::setRemoteVehicleInfo(const unsigned long &UUID,const double &Lat,const double &Lon,const float &Alt,const float &AMSL,const int &GPS,const int &GPS_fix_type)
{
    if (VehicleLocalFlag.value(UUID)!= 1)
    {   VehicleGPAlt.insert(UUID,Alt);
        VehicleGPLat.insert(UUID,Lat);
        VehicleGPLon.insert(UUID,Lon);
        VehicleGPAMSL.insert(UUID,AMSL);
        VehicleGPSStatus.insert(UUID,GPS);
        VehicleGPSStatus.insert(UUID,GPS_fix_type);
        qDebug() << VehicleGPAlt.value(UUID);
        qDebug() << "Сетевое значение";
        return;
    }
    qDebug() << "Локальное значение";
}

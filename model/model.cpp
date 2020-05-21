#include "model.h"

Model::Model()
{
    connect(&timer, &QTimer::timeout, this, &Model::TimeStampCheck);
    connect(&timer, &QTimer::timeout, this, &Model::sendTimer);
    connect(&timer, &QTimer::timeout, this, &Model::checkPossition);
    //connect(&timer_to_go, &QTimer::timeout, this, &Model::checkPossition);
    timer.start(1000);
    //timer_to_go.start(20000);
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
                              VehicleBattery.value(local_UUID),
                              VehicleLiderFlag.value(local_UUID),
                              VehicleNumber.value(local_UUID),
                              VehicleFormation.value(local_UUID),
                              VehicleAngle.value(local_UUID));
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
            VehicleLiderFlag.remove(key);
            VehicleNumber.remove(key);
            VehicleFormation.remove(key);
            VehicleAngle.remove(key);
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

void Model::setLocalVehicleAngle(const unsigned long &UUID,
                                 const float &angle_yaw)
{
    VehicleAngle.insert(UUID,angle_yaw);
    local_UUID = UUID;
    VehicleLocalFlag.insert(UUID,1);
    VehicleTimeStamp.insert(UUID,QDateTime::currentDateTime());
}

void Model::setRemoteVehicleInfo(const unsigned long &UUID,
                                 const double &Lat,
                                 const double &Lon,
                                 const float &Alt,
                                 const float &AMSL,
                                 const int &GPS_num,
                                 const int &GPS_fix_type,
                                 const float &Battery,
                                 const int &Lider,
                                 const int &Number,
                                 const int &Formation,
                                 const float &angle_yaw)
{
    if (local_UUID != UUID)
    {
        if (Lider == 1)
        {
            lider_UUID = UUID;
            VehicleLiderFlag.insert(UUID,Lider);
        }
        VehicleGPLat.insert(UUID,Lat);
        VehicleGPLon.insert(UUID,Lon);
        VehicleGPAlt.insert(UUID,Alt);
        VehicleGPAMSL.insert(UUID,AMSL);
        VehicleGPSStatus.insert(UUID,GPS_num);
        VehicleGPS_fix_type.insert(UUID,GPS_fix_type);
        VehicleBattery.insert(UUID,Battery);
        VehicleLiderFlag.insert(UUID,Lider);
        VehicleNumber.insert(UUID,Number);
        VehicleFormation.insert(UUID,Formation);
        VehicleAngle.insert(UUID,angle_yaw);

        VehicleTimeStamp.insert(UUID,QDateTime::currentDateTime());
        qDebug() << "Сетевое значение";


        //newCoordSet(Lat, Lon); //?????
        //return; //????
    }
}

void Model::checkPossition()
{
    if (local_UUID != lider_UUID && local_UUID != 0)
    {
        double targetLat = VehicleGPLat.value(lider_UUID) + (00.0000125 * 2) * qCos(VehicleAngle.value(lider_UUID)); //x
        double targetLon = VehicleGPLon.value(lider_UUID) + (00.0000010 * 2) * qCos(VehicleAngle.value(lider_UUID)); //y
        //double targetLat = 44.0769288 + (00.0000125 * 2) * qCos(VehicleAngle.value(lider_UUID)); //x
        //double targetLon = 43.0879335 + (00.0000010 * 2) * qCos(VehicleAngle.value(lider_UUID)); //y
        float targetAMSL = VehicleGPAMSL.value(lider_UUID); //z
        float targetYaw = VehicleAngle.value(lider_UUID); //yaw
        emit goToPosition (targetLat,targetLon,targetAMSL,targetYaw);
        qDebug () << "Начал движение: " << targetLat << targetLon << targetAMSL << targetYaw;
        //00,0000125 x
        //00,0000010 y
    }
}

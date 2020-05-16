#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QDebug>

class Model : public QObject
{
    Q_OBJECT

public:
    Model();

    QMultiMap <unsigned long,double> VehicleGPLat;
    QMultiMap <unsigned long,double> VehicleGPLon;
    QMultiMap <unsigned long,float> VehicleGPAlt;
    QMultiMap <unsigned long,float> VehicleGPAMSL;
    QMultiMap <unsigned long,int> VehicleGPSStatus;

    int UUID;
    double GlobalPositionLat;
    double GlobalPositionLon;
    float GlobalPositionAlt;
    float GlobalPositionAMSL;
    int GPSStatus;

    int getUUID () {return UUID;}
    void setGlobalPositionLat (int UUID_SET) {UUID = UUID_SET; }
    double getGlobalPositionLat () {return GlobalPositionLat;}
    void setGlobalPositionLat (double GlobalPositionLat_SET) {GlobalPositionLat = GlobalPositionLat_SET; }
    double getGlobalPositionLon () {return GlobalPositionLon;}
    void setGlobalPositionLon (double GlobalPositionLon_SET) {GlobalPositionLon = GlobalPositionLon_SET; }
    double getGlobalPositionAlt () {return GlobalPositionAlt;}
    void setGlobalPositionAlt (double GlobalPositionAlt_SET) {GlobalPositionAlt = GlobalPositionAlt_SET; }
    double getGlobalPositionAMSL () {return GlobalPositionAMSL;}
    void setGlobalPositionAMSL (double GlobalPositionAMSL_SET) {GlobalPositionAMSL = GlobalPositionAMSL_SET; }

public slots:
    void setLocalVehicleInfo(const unsigned long &UUID,const double &Lat,const double &Lon,const float &Alt,const float &AMSL,const int &GPS, const int &GPS_fix_type);
    void setRemoteVehicleInfo(const unsigned long &UUID,const double &Lat,const double &Lon,const float &Alt,const float &AMSL,const int &GPS, const int &GPS_fix_type);

    //double CONTROLLERgetGlobalPositionLat () {return cm->GlobalPositionLat;}
    //void CONTROLLERsetGlobalPositionLat (double GlobalPositionLat_SET) {cm->GlobalPositionLat = GlobalPositionLat_SET; }
};


#endif // MODEL_H

#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QDebug>
#include <QDateTime>
#include <QTimer>

class Model : public QObject
{
    Q_OBJECT

public:
    Model();

    QMap <unsigned long,double> VehicleGPLat;
    QMap <unsigned long,double> VehicleGPLon;
    QMap <unsigned long,float> VehicleGPAlt;
    QMap <unsigned long,float> VehicleGPAMSL;
    QMap <unsigned long,int> VehicleGPSStatus;
    QMap <unsigned long,int> VehicleGPS_fix_type;
    QMap <unsigned long,float> VehicleBattery;
    QMap <unsigned long,int> VehicleLocalFlag;
    QMap <unsigned long,QDateTime> VehicleTimeStamp;

    int UUID;
    double GlobalPositionLat;
    double GlobalPositionLon;
    float GlobalPositionAlt;
    float GlobalPositionAMSL;
    int GPSStatus;

    QTimer timer;

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

signals:
    void newCoordSet(double, double);

public slots:
    void setLocalVehiclePositionInfo(const unsigned long &UUID,const double &Lat,const double &Lon,const float &Alt,const float &AMSL);
    void setLocalVehicleGPSInfo(const unsigned long &UUID,const int &GPS_num,const int &GPS_fix_type);
    void setLocalVehicleBatteryInfo(const unsigned long &UUID,const float &Battery);
    void setRemoteVehicleInfo(const unsigned long &UUID,const double &Lat,const double &Lon,const float &Alt,const float &AMSL,const int &GPS, const int &GPS_fix_type);

private slots:
    void startTimerTimeStampCheck();
    void TimeStampCheck();
    //double CONTROLLERgetGlobalPositionLat () {return cm->GlobalPositionLat;}
    //void CONTROLLERsetGlobalPositionLat (double GlobalPositionLat_SET) {cm->GlobalPositionLat = GlobalPositionLat_SET; }
};


#endif // MODEL_H

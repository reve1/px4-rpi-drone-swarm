#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QDebug>
#include <QDateTime>
#include <QTimer>
#include <QtMath>

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
    QMap <unsigned long,float> VehicleAngle;
    QMap <unsigned long,int> VehicleLocalFlag;
    QMap <unsigned long,int> VehicleLiderFlag;
    QMap <unsigned long,int> VehicleNumber;
    QMap <unsigned long,int> VehicleFormation;
    QMap <unsigned long,QDateTime> VehicleTimeStamp;

    int UUID;
    double GlobalPositionLat;
    double GlobalPositionLon;
    float GlobalPositionAlt;
    float GlobalPositionAMSL;
    int GPSStatus;

    unsigned long local_UUID;
    unsigned long lider_UUID = 5283920058631409231;

    QTimer timer;
    QTimer timer_to_go;

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
    void sendLocalVehicleInfo(unsigned long, double, double,float,float,int,int,float,int,int,int,float);
    void goToPosition(double,double,float,float);

public slots:
    void setLocalVehiclePositionInfo(const unsigned long &UUID,
                                     const double &Lat,
                                     const double &Lon,
                                     const float &Alt,
                                     const float &AMSL);

    void setLocalVehicleGPSInfo(const unsigned long &UUID,
                                const int &GPS_num,
                                const int &GPS_fix_type);

    void setLocalVehicleBatteryInfo(const unsigned long &UUID,
                                    const float &Battery);

    void setLocalVehicleAngle(const unsigned long &UUID,
                              const float &angle_yaw);

    void setRemoteVehicleInfo(const unsigned long &UUID,
                              const double &Lat,
                              const double &Lon,
                              const float &Alt,
                              const float &AMSL,
                              const int &GPS,
                              const int &GPS_fix_type,
                              const float &Battery,
                              const int &Lider,
                              const int &Number,
                              const int &Formation,
                              const float &angle_yaw);

private slots:
    void sendTimer();
    void TimeStampCheck();
    void checkPossition();
    //double CONTROLLERgetGlobalPositionLat () {return cm->GlobalPositionLat;}
    //void CONTROLLERsetGlobalPositionLat (double GlobalPositionLat_SET) {cm->GlobalPositionLat = GlobalPositionLat_SET; }
};


#endif // MODEL_H

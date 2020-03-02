#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QDebug>

class Model : public QObject
{
    Q_OBJECT

public:
    Model();

    struct DeviceModel
    {
        QList<QString> BluetoothParametrsName;
        QList<int> BluetoothParametrsRSSI;
        //QList<int> ControllerParametrs;
    };

    struct ControllerModel
    {
        double GlobalPositionLat;
        double GlobalPositionLon;
        float GlobalPositionAlt;
        float GlobalPositionAMSL;
    };

    int UUID;
    double GlobalPositionLat;
    double GlobalPositionLon;
    float GlobalPositionAlt;
    float GlobalPositionAMSL;

    double getGlobalPositionLat () {return GlobalPositionLat;}
    void setGlobalPositionLat (double GlobalPositionLat_SET) {GlobalPositionLat = GlobalPositionLat_SET; }
    double getGlobalPositionLon () {return GlobalPositionLon;}
    void setGlobalPositionLon (double GlobalPositionLon_SET) {GlobalPositionLon = GlobalPositionLon_SET; }
    double getGlobalPositionAlt () {return GlobalPositionAlt;}
    void setGlobalPositionAlt (double GlobalPositionAlt_SET) {GlobalPositionAlt = GlobalPositionAlt_SET; }
    double getGlobalPositionAMSL () {return GlobalPositionAMSL;}
    void setGlobalPositionAMSL (double GlobalPositionAMSL_SET) {GlobalPositionAMSL = GlobalPositionAMSL_SET; }
};


#endif // MODEL_H

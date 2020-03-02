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

    double getGlobalPositionLat () {return UUID;}
    void setGlobalPositionLat (double GlobalPositionLat_SET) {UUID = GlobalPositionLat_SET; }
};


#endif // MODEL_H

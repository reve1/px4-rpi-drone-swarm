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
        int GlobalPositionLat;
        int GlobalPositionLon;
        int GlobalPositionAlt;
    };
};


#endif // MODEL_H

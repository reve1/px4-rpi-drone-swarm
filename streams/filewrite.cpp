#include "filewrite.h"

FileWrite::FileWrite()
{
}

void FileWrite::WriteFromClass(int flag, QString data)
{
#if defined (Q_OS_ANDROID)
        QString path = "/storage/emulated/0/";      //Android
#elif defined (Q_OS_WIN)
        QString path = "C:\\Users\\qt\\";           //Win
#else
        QString path = "/var/log/s500/";            //linux-RPi
#endif
    QString name;
    switch(flag)
    {
    case 0:
        name = "main.txt";
        break;
    case 1:
        name = "bluetoothserver.txt";
        break;
    case 2:
        name = "bluetoothclient.txt";
        break;
    case 3:
        name = "bluetoothchat.txt";
        break;
    case 4:
        name = "bluetoothdiscovery.txt";
        break;
    case 5:
        name = "mavlink.txt";
        break;
    default:
        break;
    }

    QFile file(path + QDateTime::currentDateTime().toString("dd.MM.yyyy") + "_" + name);
    if (!file.open(QIODevice::Append))
    {
        qCritical() << "ERROR";
    }
    else
    {
        QTextStream stream( &file );
        stream << QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm:ss    ") + data;
        stream << "\r\n";
    }
}

FileWrite::~FileWrite()
{
}

#ifndef FILEWRITE_H
#define FILEWRITE_H

#include <QObject>
#include <QFile>
#include <QString>
#include <QDebug>
#include <QThread>
#include <QDateTime>


class FileWrite : public QObject
{
    Q_OBJECT

public:
    FileWrite();
    ~FileWrite();
    static void WriteFromClass(int flag, QString data);
};

#endif // FILEWRITE_H

#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <fstream>
#include <QObject>
#include <QDebug>
#include <QTextCodec>
#include <QString>
#include <QProcess>
#include <QDir>
#include <QFile>
#include <QDataStream>

constexpr unsigned int str2int(const char* str, int h = 0)
{
    return !str[h] ? 5381 : (str2int(str, h+1) * 33) ^ str[h];
}

class Utils : public QObject
{
    Q_OBJECT
public:

    explicit Utils(QObject *parent = 0);

signals:

public slots:

private:

};

#endif // UTILS_H

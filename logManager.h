#ifndef LOGMANAGER_H
#define LOGMANAGER_H

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

namespace LogManager {

class LogManager : public QObject
{
    Q_OBJECT
public:

    explicit LogManager(QObject *parent = 0);

signals:

public slots:

private:

};

static const LogManager logManager;

static void Write(std::string data);

}
#endif // LOGMANAGER_H

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

#include "settings.h"

namespace LogManager {

    class LogManager : public QObject
    {
        Q_OBJECT
    public:

        std::clock_t startClock;
        explicit LogManager(QObject *parent = 0);

        void Write(std::string message);

    signals:

    public slots:

    private:

    };
}
#endif // LOGMANAGER_H

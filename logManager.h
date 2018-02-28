#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include <fstream>
#include <iostream>
#include <sstream>

#include <QObject>
#include <QDebug>
#include <QTextCodec>
#include <QString>
#include <QProcess>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QDateTime>
#include <QDataStream>

#include "settings.h"

namespace LogManager {

    class LogManager : public QObject
    {
        Q_OBJECT
    public:
        explicit LogManager(QObject *parent = 0);

        void Write(QString inputData);
        void InitializeLog();

    signals:

    public slots:

    private:

    };
}
#endif // LOGMANAGER_H

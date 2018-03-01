#ifndef LOGGING_H
#define LOGGING_H

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

namespace Pico{
    namespace Logging{

    class Funcs : public QObject
    {
        Q_OBJECT
        public:
            explicit Funcs(QObject *parent = 0);

            void Write(QString inputData);
            void InitializeLog();

        signals:

        public slots:

        private:

        };
    }
}
#endif // LOGGING_H

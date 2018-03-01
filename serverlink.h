#ifndef SOCKETTEST_H
#define SOCKETTEST_H

#include <iostream>

#include <QObject>
#include <QTcpSocket>
#include <QDebug>
#include <QTextCodec>
#include <QDataStream>
#include <QJsonObject>
#include <QJsonDocument>

#include "logging.h"
#include "settings.h"

namespace Pico{
    namespace Server{

    class Funcs : public QObject
    {
        Q_OBJECT
        public:
            explicit Funcs(QObject *parent = 0);

            QTcpSocket socket;

            bool IsConnected();
            QString ReceiveData();
            void CloseConnection();
            void SendData(QString data);

        signals:

        public slots:

        private:

        };
    }
}

#endif // SOCKETTEST_H

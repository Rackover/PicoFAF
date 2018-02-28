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

namespace SocketManager {

    class SocketManager : public QObject
    {
        Q_OBJECT
    public:
        explicit SocketManager(QObject *parent = 0);

        QTcpSocket* socket;

        QTcpSocket* InitalizeConnection();
        bool IsConnected(QTcpSocket* socket);
        QString ReceiveData(QTcpSocket* socket);
        void CloseConnection(QTcpSocket* socket);
        void SendData(QTcpSocket* socket, QString data);

    signals:

    public slots:

    private:

    };
}

#endif // SOCKETTEST_H

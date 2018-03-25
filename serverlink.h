#ifndef SOCKETTEST_H
#define SOCKETTEST_H

#include <QString>
#include <QTcpSocket>
#include <QJsonObject>

namespace Pico{
    namespace Server{

    class Funcs : public QObject
    {
        Q_OBJECT
        public:
            explicit Funcs(QObject *parent = 0);

            QTcpSocket socket;

            QString MakeHelloMessage();
            void SendData(QString data);

            bool IsConnected();
            QString GetCommand(QJsonObject serverData);
            void LogIn(QJsonObject serverData);
            QJsonObject JsonDecode(QString jsonString);
            void CloseConnection(QString errorMsg="No error message.");
            QString IsolateMessage();

        signals:
            void AddedGame(int id, QJsonObject game);
            void DeletedGame(int id);
            void Connected();
            void Disconnected();
            void StatusChanged(QString status);

        public slots:
            void OnReceivedData();
            void OnClosedConnection();
            void OnConnected();
            void OnSocketError(QAbstractSocket::SocketError error);

        private:

        };
    }
}

#endif // SOCKETTEST_H

#include "socketManager.h"

#include "logManager.h"
#include "settings.h"


namespace SocketManager {

    LogManager::LogManager logs;

    FD_CONFIG FDC;

    SocketManager::SocketManager(QObject *parent) :
        QObject(parent)
    {
        socket = new QTcpSocket(this);
        socket->connectToHost(QString::fromStdString(FDC.SERVER_ADDRESS), (quint16) FDC.SERVER_PORT);
        socket->setSocketOption(QAbstractSocket::KeepAliveOption, 1);
    }



    QTcpSocket* SocketManager::InitalizeConnection()
    {
        if(socket->waitForConnected(3000))
        {
            logs.Write("INITIALIZE_CONNECTION => Connected!");

            QJsonObject helloObject{
                {"command", "ask_session"},
                {"version", QString::fromStdString(HC_CONFIG.CLIENT_VERSION)},
                {"user_agent", "faf-client"}
            };

            QJsonDocument helloDocument(helloObject);
            QString helloMessage = helloDocument.toJson();

           logs.Write("INITIALIZE_CONNECTION => Sending  " + helloMessage.toStdString());

           QDataStream socketStream{socket};
           socketStream.setVersion(QDataStream::Qt_4_2);

           socketStream << (quint32) helloMessage.size()*2 + 4 << helloMessage;

           socket->flush();

        }
        else
        {
            logs.Write("INITIALIZE_CONNECTION => Not connected!");
        }
        return socket;
    }

    bool SocketManager::IsConnected(QTcpSocket* socket){
        return (socket->state() == QTcpSocket::ConnectedState);
    }

    QString SocketManager::ReceiveData(QTcpSocket* socket){

        QString msg;

        while(socket->waitForReadyRead(1000)) {
            if(socket->bytesAvailable() < 4) continue;

            QDataStream socketStream{socket};
            socketStream.setVersion(QDataStream::Qt_4_2);

            quint32 bs;
            socketStream >> bs;

            socketStream >> msg;

            logs.Write("RECEIVE_DATA => Received message [" + msg.length() + std::string("]: ") + msg.toStdString());

            return msg;

        }

        return msg;
    }
    void SocketManager::CloseConnection(QTcpSocket* socket){

            socket->close();
            logs.Write("CLOSE_CONNECTION => Closed socket");
    }



    void SocketManager::SendData(QTcpSocket* socket, QString data){

        QDataStream socketStream{socket};
        socketStream.setVersion(QDataStream::Qt_4_2);

        socketStream << (quint32) data.size()*2 + 4 << data;

        socket->flush();
        logs.Write("SEND_DATA => Data sent");
    }
}

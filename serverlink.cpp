#include "serverlink.h"

#include "logging.h"
#include "settings.h"
#include "authentification.h"

#include <QDataStream>
#include <QJsonDocument>
#include <QJsonObject>

namespace Pico{
    namespace Server{

        Pico::Logging::Funcs logging;

        Pico::Settings::FD_CONFIG FDC;

        Funcs::Funcs(QObject *parent) :
            QObject(parent)
        {


            ///
            /// Connecting the wires for async system
            ///
            qDebug() << connect(&socket,
                    SIGNAL(connected()),
                    SLOT(OnConnected()));

            qDebug() << connect(&socket,
                    SIGNAL(error(QAbstractSocket::SocketError)),
                    SLOT(OnSocketError(QAbstractSocket::SocketError)));

            qDebug() << connect(&socket,
                    &socket.readyRead(),
                    Funcs::OnReceivedData());

            qDebug() << connect(&socket,
                    SIGNAL(disconnected()),
                    SLOT(OnClosedConnection()));

            logging.Write("INITIALIZE_CONNECTION => Connecting to "+QString::fromStdString(FDC.SERVER_ADDRESS)+":"+QString::number(FDC.SERVER_PORT)+"...");

            socket.connectToHost(QString::fromStdString(FDC.SERVER_ADDRESS), (quint16) FDC.SERVER_PORT);
            socket.setSocketOption(QAbstractSocket::KeepAliveOption, 1);

        }

        bool Funcs::IsConnected(){
            return (socket.state() == QTcpSocket::ConnectedState);
        }


        void Funcs::CloseConnection(QString errorMsg){

                socket.close();
                if (errorMsg.length() >0){
                    emit StatusChanged("Closed connection : "+errorMsg);
                }
                logging.Write("CLOSE_CONNECTION => Connection closed with error message : "+errorMsg+".");
        }



        void Funcs::SendData(QString data){

            QTcpSocket* socketPointer = &socket;
            QDataStream socketStream{socketPointer};

            socketStream.setVersion(QDataStream::Qt_4_2);

            socketStream << (quint32) data.size()*2 + 4 << data;

            socket.flush();
            logging.Write("SEND_DATA => Data sent");
        }

        void Funcs::OnSocketError(QAbstractSocket::SocketError error){
            logging.Write("ON_SOCKET_ERROR => Error : "+socket.errorString());
        }

        void Funcs::OnReceivedData(){

            logging.Write("ON_RECEIVED_DATA => Triggered");

            QString msg = IsolateMessage();

            if (msg.length() > 0){
                logging.Write(QString("ON_RECEIVED_DATA => Received non-empty message [") + QString::number(msg.length()) + QString("]: ") + msg);
            }
            else{
                return;
            }

            if (msg == "PING"){
                logging.Write("ON_RECEIVED_DATA => Pong-ing back.");
                SendData("PONG");
            }

            QJsonObject jsonMsg = JsonDecode(msg);
            QString command = GetCommand(jsonMsg);

            if (command == "session"){
                LogIn(jsonMsg);
            }

            else if (command == "authentication_failed"){
                QString errorMsg = jsonMsg.value("text").toString();
                CloseConnection(errorMsg);
            }

            else if (command == "game_info"){
                QString state = jsonMsg.value("state").toString();
                int key = jsonMsg.value("uid").toInt();

                if (state == "open"){
                    emit AddedGame(key, jsonMsg);
                }
                else{
                    emit DeletedGame(key);
                }
            }


        }

        void Funcs::OnClosedConnection(){

            emit Disconnected();

            logging.Write("ON_CLOSED_CONNECTION => Connection closed.");
        }

        void Funcs::OnConnected(){
            logging.Write("ON_CONNECTED => Connected to "+QString::fromStdString(FDC.SERVER_ADDRESS)+"!");

            emit Connected();
            emit StatusChanged("Connected to the server");

            QString helloMessage = MakeHelloMessage();
            SendData(helloMessage);
            logging.Write(QString("ON_CONNECTED => Sending  ") + helloMessage);
        }

        ////
        /// LOG IN
        ////
        void Funcs::LogIn(QJsonObject serverData){

            double sessionId =  serverData.value("session").toDouble();
            logging.Write("LOG_IN => Generating UID...");
            QString sessionUid = Pico::Auth::GetUid(sessionId);

            if (sessionUid == "-1"){
                logging.Write(QString::fromStdString("LOG_IN => COULD NOT GENERATE UID. CLOSING CONNECTION."));
                emit StatusChanged("Could not generate UID. Closed connection");
                CloseConnection();
            }

            QJsonObject responseObject{
                {"command", "hello"},
                {"login", QString::fromStdString(FDC.LOGIN)},
                {"unique_id", sessionUid},
                {"session", sessionId},
                {"password", QString::fromStdString(FDC.HASHWORD)} };

            QJsonDocument responseDocument(responseObject);

            SendData(QString(responseDocument.toJson()));
            emit StatusChanged("Logged in");

        }


        ////
        /// Returns wellformed hello message
        ////
        QString Funcs::MakeHelloMessage(){
            QJsonObject helloObject{
                {"command", "ask_session"},
                {"version", QString::fromStdString(Pico::Settings::HC_CONFIG.CLIENT_VERSION)},
                {"user_agent", "faf-client"}
            };

            QJsonDocument helloDocument(helloObject);
            QString helloMessage = helloDocument.toJson();

            return helloMessage;
        }


        ////
        /// Isolates COMMAND from the servers gibberish
        ////
        QString Funcs::GetCommand(QJsonObject serverData){
            QString command = serverData.value("command").toString();
            return command;
        }


        ////
        /// Json string to QJsonObject
        ////
        QJsonObject Funcs::JsonDecode(QString jsonString){
            QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonString.toLocal8Bit());
            QJsonObject jsonObject = jsonDocument.object();
            return jsonObject;
        }

        ///
        /// Given the socket is readable, return its content as a string.
        ///
        QString Funcs::IsolateMessage(){
            QString msg;

            if(socket.bytesAvailable() >= 4){

                QTcpSocket* socketPointer = &socket;

                QDataStream socketStream{socketPointer};

                socketStream.setVersion(QDataStream::Qt_4_2);

                quint32 bs;
                socketStream >> bs;

                socketStream >> msg;

            }

            return msg;
        }
    }
}

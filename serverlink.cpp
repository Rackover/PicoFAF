#include "serverlink.h"

#include "logging.h"
#include "settings.h"


namespace Pico{
    namespace Server{

        Pico::Logging::Funcs logging;

        FD_CONFIG FDC;

        Funcs::Funcs(QObject *parent) :
            QObject(parent)
        {
            socket.connectToHost(QString::fromStdString(FDC.SERVER_ADDRESS), (quint16) FDC.SERVER_PORT);
            socket.setSocketOption(QAbstractSocket::KeepAliveOption, 1);

            if(socket.waitForConnected(3000))
            {
                logging.Write("INITIALIZE_CONNECTION => Connected!");

                QJsonObject helloObject{
                    {"command", "ask_session"},
                    {"version", QString::fromStdString(HC_CONFIG.CLIENT_VERSION)},
                    {"user_agent", "faf-client"}
                };

                QJsonDocument helloDocument(helloObject);
                QString helloMessage = helloDocument.toJson();

               logging.Write(QString("INITIALIZE_CONNECTION => Sending  ") + helloMessage);

               QDataStream socketStream(&socket);
               socketStream.setVersion(QDataStream::Qt_4_2);

               socketStream << (quint32) helloMessage.size()*2 + 4 << helloMessage;

               socket.flush();

            }
            else
            {
                logging.Write("INITIALIZE_CONNECTION => Not connected!");
            }
        }
        bool Funcs::IsConnected(){
            return (socket.state() == QTcpSocket::ConnectedState);
        }

        QString Funcs::ReceiveData(){

            QString msg;

            while(socket.waitForReadyRead(1000)) {
                if(socket.bytesAvailable() < 4) continue;

                QTcpSocket* socketPointer = &socket;

                QDataStream socketStream{socketPointer};

                socketStream.setVersion(QDataStream::Qt_4_2);

                quint32 bs;
                socketStream >> bs;

                socketStream >> msg;


                logging.Write(QString("RECEIVE_DATA => Received message [") + msg.length() + QString("]: ") + msg);

                return msg;

            }

            return msg;
        }
        void Funcs::CloseConnection(){

                socket.close();
                logging.Write("CLOSE_CONNECTION => Closed socket");
        }



        void Funcs::SendData(QString data){

            QTcpSocket* socketPointer = &socket;
            QDataStream socketStream{socketPointer};

            socketStream.setVersion(QDataStream::Qt_4_2);

            socketStream << (quint32) data.size()*2 + 4 << data;

            socket.flush();
            logging.Write("SEND_DATA => Data sent");
        }
    }
}

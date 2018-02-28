#include "responseManager.h"
#include "gameManager.h"
#include "utils.h"
#include "settings.h"
#include "logManager.h"
#include "socketManager.h"
#include "authentificationManager.h"
namespace ResponseManager {
    ResponseManager::ResponseManager(QObject *parent) :
        QObject(parent)
    {
    }

    LogManager::LogManager logs;


    FD_CONFIG FDC;
    GameManager gameApp;

    QString ResponseManager::MakeResponse(QString inputData){

        if (inputData == "PING"){
            logs.Write("MAKE_RESPONSE => Pong-ing back.");
            return "PONG";
        }

        QString response = "";

        QJsonDocument jsonDocument = QJsonDocument::fromJson(inputData.toLocal8Bit());

        QJsonObject jsonObject = jsonDocument.object();

        QString command = jsonObject.value("command").toString();

        switch (str2int(command.toStdString().data())){

            ///WILL TRY TO LOG IN AUTOMATICALLY
            case str2int("session"):
                {
                    double sessionId =  jsonObject.value("session").toDouble();
                    QString sessionUid = AuthentificationManager::GetUid(sessionId);

                    QJsonObject responseObject{
                        {"command", "hello"},
                        {"login", QString::fromStdString(FDC.LOGIN)},
                        {"unique_id", sessionUid},
                        {"session", sessionId},
                        {"password", QString::fromStdString(FDC.HASHWORD)} };

                    QJsonDocument responseDocument(responseObject);
                    response = responseDocument.toJson();
                    break;
                }
            ////

            case str2int("game_info"):
                {
                    qDebug() << jsonObject.value("state");
                    //GameManager.UpdateGame(jsonObject.value("uid"), jsonObject);
                    break;
                }

        }
        if (response.length() > 0){
           logs.Write(QString::fromStdString("MAKE_RESPONSE => ") + response);
        }
        return response;
    }

    /*
     *  while (socket->state() == socket->ConnectedState){



             }

            qDebug() << "Closed socket";
            socket->close();
            */
}

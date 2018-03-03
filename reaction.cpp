#include "reaction.h"
#include "games.h"
#include "utils.h"
#include "settings.h"
#include "logging.h"
#include "serverlink.h"
#include "authentification.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>

namespace Pico{
    namespace Reaction{
        Funcs::Funcs(QObject *parent) :
            QObject(parent)
        {}

        Pico::Logging::Funcs logging;
        Pico::Games::Funcs games;
        Pico::Settings::FD_CONFIG FDC;


        //////
        /// \brief Funcs::MakeResponse
        /// \param inputData
        /// \param gamesMap
        /// \return
        ////

        QString Funcs::MakeResponse(
                QString inputData,
                std::map<int, QJsonObject> &gamesMap
                ){

            if (inputData == "PING"){
                logging.Write("MAKE_RESPONSE => Pong-ing back.");
                return "PONG";
            }

            QString response = "";

            QJsonDocument jsonDocument = QJsonDocument::fromJson(inputData.toLocal8Bit());

            QJsonObject jsonObject = jsonDocument.object();

            QString command = jsonObject.value("command").toString();

            switch (Pico::Utils::str2int(command.toStdString().data())){

                ///WILL TRY TO LOG IN AUTOMATICALLY
                case Pico::Utils::str2int("session"):
                    {
                        double sessionId =  jsonObject.value("session").toDouble();
                        QString sessionUid = Pico::Auth::GetUid(sessionId);

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

                case Pico::Utils::str2int("game_info"):
                    {
                        QString state = jsonObject.value("state").toString();

                        if (state == "open"){
                            int key = jsonObject.value("uid").toInt();

                            gamesMap[key] = jsonObject;
                        }
                        break;
                    }

            }
            if (response.length() > 0){
               logging.Write(QString::fromStdString("MAKE_RESPONSE => ") + response);
            }
            return response;
        }
    }
}

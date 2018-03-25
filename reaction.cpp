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


        ////
        /// \brief Funcs::MakeResponse
        /// \param inputData
        /// \param gamesMap
        /// \return
        ///

        QString Funcs::MakeResponse(
                QString inputData,
                std::map<int, QJsonObject> &gamesMap,
                Pico::Server::Funcs &server
                ){


            QString response = "";

            QJsonDocument jsonDocument = QJsonDocument::fromJson(inputData.toLocal8Bit());

            QJsonObject jsonObject = jsonDocument.object();

            QString command = jsonObject.value("command").toString();
            if (response.length() > 0){
               logging.Write(QString::fromStdString("MAKE_RESPONSE => ") + response);
            }
            return response;
        }
    }
}

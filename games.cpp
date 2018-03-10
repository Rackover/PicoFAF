#include "games.h"
#include "logging.h"

#include <QtNetwork>

namespace Pico{
    namespace Games{

        Pico::Logging::Funcs logging;

        Funcs::Funcs(QObject* parent) :
            QObject(parent)
        {
        }

        void Funcs::PopulateDownloadsMap(QJsonObject game, std::map<std::string, int> downloadsMap){
            std::string mapUrl = "http://content.faforever.com/"+game.value("map_file_path").toString().toStdString();
            QJsonObject modsList = game.value("sim_mods").toObject();

            logging.Write("POPULATE_DOWNLOADS_MAP=>Preparing to list mods");

            foreach(const QString& key, modsList.keys()) {
                std::string name = modsList.value(key).toString().toStdString();
                std::string uid = key.toStdString();
                std::string modApiPage = "https://api.faforever.com/data/mod?&filter=versions.uid=="+uid+"&include=versions&fields[modVersion]=downloadUrl";

                /// Fetch download address from API using mod UID
                const QUrl url = QUrl(QString::fromStdString(modApiPage));
                const QNetworkRequest request(url);
                QNetworkAccessManager networkManager;

                connect(&networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onVaultResponse(QNetworkReply*)));



                logging.Write(data);
                abort();

            }



            //
        }

        void Funcs::onVaultResponse(QNetworkReply* reply)
        {
            if (reply->error() != QNetworkReply::NoError)
                logging.Write(data);
                return;  // ...only in a blog post

            QString data = (QString) reply->readAll();

            QScriptEngine engine;
            QScriptValue result = engine.evaluate(data);
    }
}

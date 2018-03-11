#include "downloader.h"
#include "logging.h"
#include "utils.h"

#include <QtNetwork>

#include <condition_variable>

namespace Pico{
    namespace Downloader{

    Pico::Logging::Funcs logging;
    Funcs::currentDownload download;

        Funcs::Funcs(QObject* parent) :
            QObject(parent)
        {
        }

        void Funcs::DownloadFiles(std::map<std::string, int> &downloadsMap){

        }



        void Funcs::PopulateDownloadsMap(QJsonObject game, std::map<std::string, int> downloadsMap){
            std::string mapUrl = "http://content.faforever.com/"+game.value("map_file_path").toString().toStdString();
            downloadsMap[mapUrl] = 0;

            QJsonObject modsList = game.value("sim_mods").toObject();
            logging.Write("POPULATE_DOWNLOADS_MAP => Preparing to list mods");

            foreach(const QString& key, modsList.keys()) {


                std::string name = modsList.value(key).toString().toStdString();
                std::string uid = key.toStdString();
                std::string modApiPage = "https://api.faforever.com/data/mod?page[size]=1&filter=versions.uid=="+uid+"&include=versions&fields[modVersion]=downloadUrl";

                /// Fetch download address from API using mod UID
                const QUrl url = QUrl(QString::fromStdString(modApiPage));
                const QNetworkRequest request(url);
                QNetworkAccessManager networkManager;

                connect(&networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onVaultResponse(QNetworkReply*)));

                logging.Write("POPULATE_DOWNLOADS_MAP => Waiting for the data receiving for mod "+QString::fromStdString(uid)+"...");

                std::unique_lock<std::mutex> lock(download.lock);
                download.finished.wait_for(lock, std::chrono::seconds(5));
                logging.Write("POPULATE_DOWNLOADS_MAP => Adding URL to map");

                downloadsMap[download.data.toStdString()] = 0;

            }

            logging.Write("POPULATE_DOWNLOADS_MAP => Populated map :" + QString::fromStdString(Utils::map_to_string(downloadsMap)));

            abort();


            //
        }

        void Funcs::onVaultResponse(QNetworkReply* reply)
        {
            std::unique_lock<std::mutex> lock(download.lock);
            if (reply->error() != QNetworkReply::NoError){
                QVariant statusCode = reply->attribute( QNetworkRequest::HttpStatusCodeAttribute );
                download.state = statusCode.toInt();
                logging.Write("ON_VAULT_RESPONSE => Server returned error "+QString::number(download.state));
                download.finished.notify_one();
                return;
            }

            QString data = (QString) reply->readAll();
            logging.Write("ON_VAULT_RESPONSE => Got data : "+data);
            QJsonDocument jsonData = QJsonDocument::fromJson(data.toUtf8());
            QJsonObject objectData = jsonData.object();
            download.data = objectData.value("included").toArray().at(0).toObject().value("attributes").toObject().value("downloadUrl").toString();

            logging.Write("ON_VAULT_RESPONSE => Got URL from data : "+download.data);
            download.finished.notify_one();
        }
    }
}

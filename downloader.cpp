#include "downloader.h"
#include "logging.h"
#include "utils.h"
#include "settings.h"

#include <QtNetwork>

#include <condition_variable>

namespace Pico{
    namespace Downloader{

    Pico::Logging::Funcs logging;
    Pico::Settings::FD_CONFIG FDC;

        ///
        /// Connect the wires before doing anything
        /// This way, a get will always trigger a "downloadFinished()" call
        ///
        Funcs::Funcs(QObject* parent) :
            QObject(parent)
        {
            connect(&networkManager, SIGNAL(finished(QNetworkReply*)),
                    SLOT(OnRequestFinished(QNetworkReply*)));
        }

        ///
        /// \brief Funcs::DownloadIfMissing
        /// \param url
        /// \param folder
        ///
        /// Download file only if not already in folder
        ///

        void Funcs::DownloadIfMissing(QUrl &url, QString folder){

            QString path = url.path();
            QString filename = QFileInfo(path).fileName();

            QString fullPath = QString::fromStdString(FDC.CUSTOM_PATH)+"/"+folder+"/"+filename;

            if (!QFile(fullPath).exists()){
                if (!QDir(QString::fromStdString(FDC.CUSTOM_PATH)+"/"+folder).exists()){
                    QDir().mkdir(QString::fromStdString(FDC.CUSTOM_PATH)+"/"+folder);
                }
                FireRequest(url);
            }

        }

        ///
        /// \brief Funcs::FireRequest
        /// \param url
        /// Adds request to queue
        ///

        void Funcs::FireRequest(const QUrl &url)
        {
            QNetworkRequest request(url);
            QNetworkReply *reply = networkManager.get(request);

        #if QT_CONFIG(ssl)
            connect(reply, SIGNAL(SSLErrors(QList<QSslError>)),
                    SLOT(SSLErrors(QList<QSslError>)));
        #endif

            currentDownloads.append(reply);
        }


        ///
        /// Populate downloads vector with what's needed, given the game information
        ///

        void Funcs::SynchronizeCustomData(QJsonObject game){

            if (!currentDownloads.isEmpty()){
                return;
            }

            /// Downloading map
            QUrl mapUrl = QString("http://content.faforever.com/"+game.value("map_file_path").toString());
            DownloadIfMissing(mapUrl, "maps");
            logging.Write("POPULATE_DOWNLOADS_MAP => Waiting for the data receiving for map "+game.value("map_file_path").toString()+" (address : [ "+mapUrl.toDisplayString()+" ]");

            /// Downloading every mod
            QJsonObject modsList = game.value("sim_mods").toObject();
            logging.Write("POPULATE_DOWNLOADS_MAP => Preparing to download "+QString::number(modsList.size())+" mods");
            logging.Write("POPULATE_DOWNLOADS_MAP => Modlist is : "+ QString::fromUtf8(QJsonDocument(modsList).toJson()) +"");

            foreach(const QString& key, modsList.keys()) {

                std::string name = modsList.value(key).toString().toStdString();
                std::string uid = key.toStdString();
                std::string modApiPage = "https://api.faforever.com/data/modVersion?filter=uid=="+uid+"&fields[modVersion]=downloadUrl";

                /// Fetch download address from API using mod UID
                const QUrl url = QUrl(QString::fromStdString(modApiPage));

                FireRequest(url);
                logging.Write("POPULATE_DOWNLOADS_MAP => Waiting for the data receiving for mod "+QString::fromStdString(uid)+" (address : [ "+QString::fromStdString(modApiPage)+" ]");

            }
        }



        ///
        /// Finds the download link in the server response
        ///

        QUrl Funcs::GetUrlFromAPIResponse(QString apiJson)
        {

            QString data = apiJson;
            logging.Write("DURL_FROM_API => Got data : "+data);
            QJsonDocument jsonData = QJsonDocument::fromJson(data.toUtf8());
            QJsonObject objectData = jsonData.object();
            data = objectData.value("included").toArray().at(0).toObject().value("attributes").toObject().value("downloadUrl").toString();

            logging.Write("DURL_FROM_API => Got URL from data : "+data);

            QUrl url = QUrl(data);
            return url;
        }

        ///
        /// \brief Funcs::SaveFile
        /// \param filename
        /// \param data
        /// \return
        ///
        /// Save reply content on disk
        ///

        bool Funcs::SaveFile(const QString &filename, QIODevice *data)
        {
            QFile file(filename);
            if (!file.open(QIODevice::WriteOnly)) {
                logging.Write("SAVE_FILE => "+file.errorString()+" when trying to save "+filename);
                return false;
            }

            file.write(data->readAll());
            file.close();

            logging.Write("SAVE_FILE => Successful write of "+filename);

            return true;
        }


        ////////////////
        /// SLOTS
        ////////////////



        ///
        /// Triggered when the request comes back
        ///

        void Funcs::OnRequestFinished(QNetworkReply *reply)
        {
            QUrl url = reply->url();
            if (reply->error()) {
                logging.Write("ON_REQUEST_FINISHED => Reply "+url.toString()+" returned error "+reply->errorString()+"");
            } else {
                if (IsHttpRedirect(reply)) {
                    logging.Write("ON_REQUEST_FINISHED => Reply "+url.toString()+" was redirected.");
                } else {

                    /// Decide wether its a ZIP or an API request

                    QString path = url.path();
                    QString filename = QFileInfo(path).fileName();

                    if (filename.isEmpty()){

                        /// API REQUEST

                        QString jsonString = ParseReply(reply);
                        logging.Write("ON_REQUEST_FINISHED => Received API response "+jsonString);
                        QUrl downloadUrl = GetUrlFromAPIResponse(jsonString);
                        DownloadIfMissing(downloadUrl);

                    }
                    else{

                        /// FILE DOWNLOAD

                        QString path = url.path();
                        QString basename = QFileInfo(path).fileName();

                        /// dirty, will need fixing
                        if (url.scheme() == "http"){        /// is a map
                            SaveFile(QString::fromStdString(FDC.CUSTOM_PATH)+"/"+"maps"+"/"+basename, reply);
                        }
                        else{                               /// is a mod
                            SaveFile(QString::fromStdString(FDC.CUSTOM_PATH)+"/"+"mods"+"/"+basename, reply);
                        }

                    }


                }
            }

            currentDownloads.removeAll(reply);  /// deletes reply from download list
            reply->deleteLater();

            if (currentDownloads.isEmpty()) {
                // all downloads finished
            }
        }

        ///
        /// Logs ssl errors
        ///
        void Funcs::SSLErrors(const QList<QSslError> &sslErrors)
        {
            #if QT_CONFIG(ssl)
                for (const QSslError &error : sslErrors)
                    logging.Write("SSL ERROR => "+error.errorString());
            #else
                Q_UNUSED(SSLErrors);
            #endif
        }


        ///
        /// Returns string held by reply
        ///
        QString Funcs::ParseReply(QIODevice *data)
        {
            QString textData = QString(data->readAll());

            return textData;
        }

        bool Funcs::IsHttpRedirect(QNetworkReply *reply)
        {
            int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            return statusCode == 301 || statusCode == 302 || statusCode == 303
                   || statusCode == 305 || statusCode == 307 || statusCode == 308;
        }



    }
}

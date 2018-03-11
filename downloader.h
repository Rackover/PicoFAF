#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QObject>
#include <QJsonObject>
#include <QNetworkReply>

#include <mutex>
#include <condition_variable>

namespace Pico{
    namespace Downloader{

        class Funcs : public QObject
        {
            Q_OBJECT
            public:
                explicit Funcs(QObject *parent = 0);
                void DownloadFiles(std::map<std::string, int> &downloadsMap);
                void PopulateDownloadsMap(QJsonObject game, std::map<std::string, int> downloadsMap);

                struct currentDownload{
                    std::condition_variable    finished;
                    int                     state;
                    std::string              details;
                    QString                   data;
                    std::mutex               lock;
                };


        signals:
            void onVaultResponse(QNetworkReply* reply);

        signals:

        public slots:

        private:

        };
    }
}

#endif // DOWNLOADER_H

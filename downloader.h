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
                void DownloadIfMissing(QUrl &url, QString folder="mods");
                void SynchronizeCustomData(QJsonObject game);
                bool SaveFile(const QString &filename, QIODevice *data);

                void FireRequest(const QUrl &url);
                QString ParseReply(QIODevice *data);
                bool IsHttpRedirect(QNetworkReply *reply);
                QUrl GetUrlFromAPIResponse(QString apiJson);

                QNetworkAccessManager networkManager;
                QVector<QNetworkReply *> currentDownloads;

            signals:

            public slots:
                void OnRequestFinished(QNetworkReply *reply);
                void SSLErrors(const QList<QSslError> &errors);

            private:

        };
    }
}

#endif // DOWNLOADER_H

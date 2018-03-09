#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QObject>

namespace Pico{
    namespace Downloader{

        class Funcs : public QObject
        {
            Q_OBJECT
            public:
                explicit Funcs(QObject *parent = 0);
                void DownloadFiles(std::map<std::string, int> &downloadsMap);

        signals:

        public slots:

        private:

        };
    }
}

#endif // DOWNLOADER_H

#ifndef GAMES_H
#define GAMES_H

#include <QJsonObject>
#include <QObject>

namespace Pico{
    namespace Games{

        class Funcs : public QObject
        {
            Q_OBJECT
            public:
                explicit Funcs(QObject *parent = 0);
                void PopulateDownloadsMap(QJsonObject game, std::map<std::string, int> downloadsMap);
        signals:

        public slots:

        private:

        };
    }
}

#endif // GAMES_H

#ifndef GAMES_H
#define GAMES_H

#include <QObject>
#include <QJsonObject>

namespace Pico{
    namespace Games{

        class Funcs : public QObject
        {
            Q_OBJECT
            public:
                explicit Funcs(QObject *parent = 0);

                std::map<int, QJsonObject> gamesMap;

        public slots:
            void OnGameAdd(int id, QJsonObject game);
            void OnGameDelete(int id);

        signals:
            void GamesChanged(std::map<int, QJsonObject> *gamesMap);
        };
    }
}

#endif // GAMES_H

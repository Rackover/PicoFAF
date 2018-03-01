#ifndef GAMES_H
#define GAMES_H

#include <iostream>
#include <map>

#include <QCoreApplication>
#include <QJsonObject>
#include <QObject>

namespace Pico{
    namespace Games{

        //This value
        class GameInfo : public QJsonObject {};

        class Funcs : public QObject
        {
            Q_OBJECT
            public:
                explicit Funcs(QObject *parent = 0);

                std::map<int, GameInfo> GameList;

                void UpdateGame(int id, GameInfo gameInfo);
                GameInfo GetGame(int id);

        signals:

        public slots:

        private:

        };
    }
}

#endif // GAMES_H

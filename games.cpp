#include "games.h"

#include <QJsonObject>

namespace Pico{
    namespace Games{

        Funcs::Funcs(QObject* parent) :
            QObject(parent)
        {
        }

        void Funcs::OnGameAdd(int id, QJsonObject game){
            gamesMap[id] = game;
            emit GamesChanged(gamesMap);
        }

        void Funcs::OnGameDelete(int id){
             gamesMap.erase(id);
             emit GamesChanged(gamesMap);
        }
    }
}

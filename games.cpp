#include "games.h"
#include "logging.h"

#include <QJsonObject>

namespace Pico{
    namespace Games{

        Pico::Logging::Funcs logging;

        Funcs::Funcs(QObject* parent) :
            QObject(parent)
        {
        }

        void Funcs::OnGameAdd(int id, QJsonObject game){
            logging.Write("ON_GAME_ADD => Adding or updating game ["+game.value("title").toString()+"] at id "+QString::number(id)+"");
            gamesMap[id] = game;
            emit GamesChanged(&gamesMap);
        }

        void Funcs::OnGameDelete(int id){
            logging.Write("ON_GAME_ADD => Deleting game at id "+QString::number(id)+"");
             gamesMap.erase(id);
             emit GamesChanged(&gamesMap);
        }
    }
}

#include "games.h"

namespace Pico{
    namespace Games{

        Funcs::Funcs(QObject* parent) :
            QObject(parent)
        {
        }


        void Funcs::UpdateGame(int id, GameInfo newGame){
            GameList.at(id) = newGame;
        }

        GameInfo Funcs::GetGame(int id){

            GameInfo game;

            game = GameList.at(id);

            return game;
        }
    }
}

#include "gameManager.h"

GameManager::GameManager(QObject *parent) :
    QObject(parent)
{
}

void GameManager::UpdateGame(int id, Game newGame){
    GameList->at(id) = newGame;
}

Game GameManager::GetGame(int id){

    Game game;

    game = GameList->at(id);

    return game;
}

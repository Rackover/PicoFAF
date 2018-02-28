#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <iostream>
#include <map>

#include <QCoreApplication>
#include <QJsonObject>

class Game : public QJsonObject {};

class GameManager : public QObject
{
    Q_OBJECT
public:
    explicit GameManager(QObject *parent = 0);

    std::map<int, Game>* GameList;

    void UpdateGame(int id, Game newGame);
    Game GetGame(int id);

signals:

public slots:

private:

};

#endif // GAMEMANAGER_H

#include "display.h"
#include "games.h"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <stdlib.h>

#include <QJsonArray>
#include <QJsonDocument>
#include <QDebug>

namespace Pico{
    namespace Display{

        Funcs::Funcs(QObject* parent) :
            QObject(parent)
        {
        }


        void Funcs::AddLine(std::string text){

            lines.insert(lines.end(), text);
        }

        void Funcs::Display(){

            std::system("cls");

            std::cout << "[status] " << status << std::endl << std::endl;

            for (std::list<std::string>::iterator it =lines.begin(); it!=lines.end(); ++it){
                std::cout << *it << std::endl;
            }

            lines.clear();
        }

        void Funcs::ListGames(std::map<int, QJsonObject>& gamesMap){

            int i = 0;

            std::map<int, QJsonObject> fakeMap(gamesMap);

            for (const auto& kv : fakeMap) {
                i++;

                QJsonObject data;
                if (i > 0){
                    data = kv.second;
                }
                std::ostringstream gameLineStream;

                gameLineStream
                        << std::internal << QString::number(i).toStdString() << "."
                         << " " << data.value("password_protected").toString().toStdString()
                         << " " << data.value("title").toString().toStdString()
                         << " " << QString::number(data.value("num_players").toInt()).toStdString() << "/" << QString::number(data.value("max_players").toInt()).toStdString()
                         << " " << data.value("mapname").toString().toStdString()
                            ;

                std::string gameLine(gameLineStream.str());
                AddLine(gameLine);
            }

            Display();
        }
    }
}

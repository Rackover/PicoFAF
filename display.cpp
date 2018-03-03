#include "display.h"
#include "games.h"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <stdlib.h>

#include <QJsonArray>
#include <QDebug>

namespace Pico{
    namespace Display{

        Funcs::Funcs(QObject* parent) :
            QObject(parent)
        {
        }


        void Funcs::AddLine(std::string text){

            //qDebug() << "Adding line" << QString::number(lines.size());
            lines.insert(lines.end(), text);
            //qDebug() << QString::number(lines.size());
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

            qDebug() << gamesMap.size();

            for (const auto& kv : gamesMap) {
                i++;

                QJsonObject data = kv.second;
                std::ostringstream gameLineStream;

                std::cout
                        << std::internal << std::setw(2) << data.value(QString::number(i)).toString().toStdString()
                        << std::setw(1) << data.value("password").toString().toStdString()
                        << std::setw(10) << data.value("name").toString().toStdString()
                        << std::setw(5) << QString::number(data.value("_players").toArray().count()).toStdString() << "/" << QString::number(data.value("max_players").toInt()).toStdString()
                        << std::setw(10) << data.value("map_file_path ").toString().toStdString()
                        << std::endl;

                gameLineStream << QString::number(i).toStdString();
                std::string gameLine(gameLineStream.str());
                AddLine(gameLine);
            }

            //Display();
        }
    }
}

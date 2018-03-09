#include "input.h"

#include <QJsonObject>

#include <wincon/pdcwin.h>
#include <map>

namespace Pico{
    namespace Input{

        Funcs::Funcs(QObject* parent) :
            QObject(parent)
        {
        }

        int Funcs::PickGame(std::map<int, QJsonObject> &gamesMap, WINDOW *window){

            echo();
            nodelay(stdscr, false);          /// Enables instant input

            int gamePicked=-1;
            int selection;

            wscanw(window, "%i", &selection);
            if (selection < (int)gamesMap.size()+1){
                int i = 0;
                for (const auto& kv : gamesMap) {
                    i++;
                    if (i==selection){
                        gamePicked = kv.first;
                    }
                }
            }

            noecho();
            nodelay(stdscr, true);          /// Enables instant input

            return gamePicked;
        }
    }
}

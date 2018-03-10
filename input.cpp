#include "input.h"
#include "logging.h"

#include <QJsonObject>

#include <wincon/pdcwin.h>
#include <map>

namespace Pico{
    namespace Input{

        Pico::Logging::Funcs logging;

        Funcs::Funcs(QObject* parent) :
            QObject(parent)
        {
        }

        int Funcs::PickGame(std::map<int, QJsonObject> &gamesMap, WINDOW *window){

            echo();
            nodelay(stdscr, false);          /// Enables instant input

            logging.Write("PICK_GAME=>Awaiting user input");

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

            logging.Write("POPULATE_DOWNLOADS_MAP=>User picked the game "+QString::number(gamePicked));

            return gamePicked;
        }
    }
}

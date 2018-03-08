#include "display.h"
#include "games.h"
#include "settings.h"
#include "utils.h"

#include <iostream>
#include <sstream>
#include <iomanip>

#include <wincon/pdcwin.h>
#include <panel.h>

#include <QJsonArray>
#include <QJsonDocument>
#include <QDebug>


namespace Pico{
    namespace Display{

        Funcs::Funcs(QObject* parent) :
            QObject(parent)
        {

            initscr();                      /// Initialize curses - mandatory
            noecho();                       /// Disables displaying user input on getch()
            cbreak();                       /// Enables instant input
            nodelay(stdscr, true);          /// Enables instant input
            keypad(stdscr, TRUE);           /// Detect function keys
            start_color();                  /// ENABLES COLORS
            curs_set(0);                    /// HIDE CURSORS

            PDC_set_title((Pico::Settings::HC_CONFIG.CLIENT_NAME).c_str());   /// SETS WINDOWS TITLE

            init_pair(1, COLOR_BLACK, COLOR_WHITE); /// 1: STATUS window

            ///
            /// MAIN WINDOW
            /// (displays game list, etc...)
            ///

            windows[0] = newwin(LINES-2, COLS, 0, 0); /// As big as the terminal

            ///
            /// COMMAND WINDOW
            /// (the scanF place where you can ask instructions)
            ///

            windows[1] = newwin(3, COLS, LINES - 4, 0); /// Online 1 text line, but 3 real lines : so it can be boxed.

            ///
            /// STATUS WINDOW
            /// (the scanF place where you can ask instructions)
            ///

            windows[2] = newwin(1, COLS, LINES -1, 0);
            wbkgdset(windows[2], COLOR_PAIR(1));

            ///
            /// DOWNLOAD WINDOW
            /// (Big thing; centered)
            ///

            int dwWidth = 40;
            int dwHeight = 30;
            windows[3] = newwin(dwHeight, dwWidth, LINES/2-dwHeight/2, COLS/2-dwWidth/2);

            ///
            /// PANELS
            ///

            for (int i =0; i < 3; i++){
                panels[i] = new_panel(windows[i]);
            }
        }


        void Funcs::AddLine(std::string text){

            lines.insert(lines.end(), text);
        }

        void Funcs::Display(){

            clear();

            /// MAIN WINDOW
            wclear(windows[0]);
            box(windows[0], 0, 0);
            int i = 1;
            for (std::list<std::string>::iterator it =lines.begin(); it!=lines.end(); ++it){
                mvwprintw(windows[0], i, 1, (*it).c_str());
                i++;
            }
            lines.clear();

            /// COMMAND ZONE
            wclear(windows[1]);
            box(windows[1], 0, 0);

            /// STATUS
            wclear(windows[2]);
            mvwprintw(windows[2], 0, 0, std::string("[status] "+status).c_str());

            /// DOWNLOAD
            wclear(windows[3]);
            box(windows[3], 0, 0);
            /* Show it on the screen */


            update_panels();
            doupdate();
        }

        void Funcs::DisplayGameList(std::map<int, QJsonObject>& gamesMap){

            int i = 0;

            std::map<int, QJsonObject> fakeMap(gamesMap);

            int nameLength = 16;
            int playersLength = 7;
            int mapLength = 24;
            int authorLength = 16;
            int modLength = 16;

            std::string separator = " | ";
            int totalLength =
                    nameLength
                    +playersLength
                    +mapLength
                    +authorLength
                    +modLength
                    + 5*separator.length();   //// Multiplying by the number of infos to display

            std::string horizontalLine;
            for (int i = 0; i < totalLength; i++){
                horizontalLine += "=";
            }

            std::ostringstream headerStream;
            headerStream
                    << std::internal
                     << Pico::Utils::makeLong("ID", 3)
                     << separator << Pico::Utils::makeLong("NAME", nameLength)
                     << separator << Pico::Utils::makeLong("PLAYERS", playersLength)
                     << separator << Pico::Utils::makeLong("MAP", mapLength)
                     << separator << Pico::Utils::makeLong("HOST", authorLength)
                     << separator << Pico::Utils::makeLong("MOD", modLength);

            AddLine(headerStream.str());
            AddLine(horizontalLine);

            for (const auto& kv : fakeMap) {
                i++;

                QJsonObject data;
                if (i > 0){
                    data = kv.second;
                }
                std::ostringstream gameLineStream;

                std::string gameId = QString::number(i).toStdString();
                std::string gameName = data.value("title").toString().toStdString();
                std::string gamePlayers = QString::number(data.value("num_players").toInt()).toStdString() + "/" + QString::number(data.value("max_players").toInt()).toStdString();
                std::string gameMap = data.value("mapname").toString().toStdString();
                std::string gameAuthor = data.value("host").toString().toStdString();
                std::string gameMod = data.value("featured_mod").toString().toStdString();

                gameLineStream
                        << std::internal
                         << Pico::Utils::makeLong(gameId+".", 3)
                         << separator << Pico::Utils::makeLong(gameName, nameLength)
                         << separator << Pico::Utils::makeLong(gamePlayers, playersLength)
                         << separator << Pico::Utils::makeLong(gameMap, mapLength)
                         << separator << Pico::Utils::makeLong(gameAuthor, authorLength)
                         << separator << Pico::Utils::makeLong(gameMod, modLength)
                            ;

                std::string gameLine(gameLineStream.str());
                AddLine(gameLine);
            }

            if (i==0){
                AddLine("Fetching games...");
            }

            Display();
        }
    }
}

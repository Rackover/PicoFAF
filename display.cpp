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
#include <QJsonObject>

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

            for (int i =0; i < 4; i++){
                windows[i] = newwin(0,0,0,0);
            }

            ResizeWindows();

            ///
            /// PANELS
            ///

            for (int i =0; i < 3; i++){
                panels[i] = new_panel(windows[i]);
            }
        }

        void Funcs::ResizeWindows(){

            resize_term(0,0);

            ///
            /// MAIN WINDOW
            /// (displays game list, etc...)
            ///

            windows[0] = resize_window(windows[0],LINES-2, COLS); /// As big as the terminal
            mvwin(windows[0], 0,0);

            ///
            /// COMMAND WINDOW
            /// (the scanF place where you can ask instructions)
            ///

            windows[1] = resize_window(windows[1], 3, COLS); /// Online 1 text line, but 3 real lines : so it can be boxed.
            mvwin(windows[1], LINES - 4, 0);

            ///
            /// STATUS WINDOW
            /// (the scanF place where you can ask instructions)
            ///

            windows[2] = resize_window(windows[2], 1, COLS);
            wbkgdset(windows[2], COLOR_PAIR(1));
            mvwin(windows[2], LINES -1, 0);

            ///
            /// DOWNLOAD WINDOW
            /// (Big thing; centered)
            ///

            int dwWidth = 40;
            int dwHeight = 30;
            windows[3] = resize_window(windows[3], dwHeight, dwWidth);
            mvwin(windows[3], LINES/2-dwHeight/2, COLS/2-dwWidth/2);
        }

        void Funcs::AddLine(std::string text){

            lines.insert(lines.end(), text);
        }

        std::string Funcs::BoolToCheckbox(bool value){
            if (value){
                return "X";
            }
            return " ";
        }

        void Funcs::Display(){
            ResizeWindows();
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
            mvwprintw(windows[1], 1, 1, std::string(command).c_str());

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

        void Funcs::RefreshCommand(std::string command){
            /// COMMAND ZONE
            wclear(windows[1]);
            box(windows[1], 0, 0);
            mvwprintw(windows[1], 1, 1, command.c_str());
            wrefresh(windows[1]);
        }

        void Funcs::DisplayGameList(std::map<int, QJsonObject>& gamesMap){

            int i = 0;

            std::map<int, QJsonObject> fakeMap(gamesMap);

            int nameLength = 16;
            int playersLength = 7;
            int mapLength = 24;
            int authorLength = 16;
            int modLength = 16;
            int passLength = 5;

            std::string separator = " | ";
            int totalLength =
                    nameLength
                    +playersLength
                    +mapLength
                    +authorLength
                    +modLength
                    +passLength
                    + 6*separator.length();   //// Multiplying by the number of infos to display

            std::string horizontalLine;
            for (int j = 0; j < totalLength; j++){
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
                     << separator << Pico::Utils::makeLong("MOD", modLength)
                     << separator << Pico::Utils::makeLong("PASS", passLength);

            AddLine(headerStream.str());
            AddLine(horizontalLine);

            for (const auto& kv : fakeMap) {
                i++;

                QJsonObject data;
                data = kv.second;
                std::ostringstream gameLineStream;

                std::string gameId = QString::number(i).toStdString();
                std::string gameName = data.value("title").toString().toStdString();
                std::string gamePlayers = QString::number(data.value("num_players").toInt()).toStdString() + "/" + QString::number(data.value("max_players").toInt()).toStdString();
                std::string gameMap = data.value("mapname").toString().toStdString();
                std::string gameAuthor = data.value("host").toString().toStdString();
                std::string gameMod = data.value("featured_mod").toString().toStdString();
                std::string gamePass = BoolToCheckbox(data.value("password_protected").toBool());
                //std::string gameMod = std::to_string(LINES);

                gameLineStream
                        << std::internal
                         << Pico::Utils::makeLong(gameId, 3)
                         << separator << Pico::Utils::makeLong(gameName, nameLength)
                         << separator << Pico::Utils::makeLong(gamePlayers, playersLength)
                         << separator << Pico::Utils::makeLong(gameMap, mapLength)
                         << separator << Pico::Utils::makeLong(gameAuthor, authorLength)
                         << separator << Pico::Utils::makeLong(gameMod, modLength)
                         << separator << Pico::Utils::makeLong(gamePass, passLength)
                            ;

                std::string gameLine(gameLineStream.str());
                AddLine(gameLine);
            }

            if (i==0){
                AddLine("Fetching games...");
            }

            Display();
        }

        void Funcs::DisplayDownloads(QVector<QNetworkReply *> currentDownloads){

        }

        ////////////////
        /// SLOTS
        ////////////////

        void Funcs::OnStatusChange(QString newStatus){
            status = newStatus.toStdString();
            Display();
        }
        void Funcs::OnGameUpdate(std::map<int, QJsonObject> &gamesMap){
            DisplayGameList(gamesMap);
        }
    }
}

#include "settings.h"
#include "reaction.h"
#include "serverlink.h"
#include "logging.h"
#include "display.h"
#include "input.h"
#include "games.h"
#include "downloader.h"

#include <iostream>

#include <QCoreApplication>

///////////////
///
/// Initialization of vars
///
///////////////

const float refreshSpeed = 1; /// One second.

std::map<std::string, int> downloadsMap;
std::map<int, QJsonObject> gamesMap;
float lastRefresh = 0;
int mode = 0;   // MODE 0 : LIST GAMES. 1 : PICK GAME. 2 : JOINING GAME.
int gamePick;   /// Used in game picking

///////////////
///
/// Start of main loop
///
///////////////

int main(int argc, char *argv[])
{
    //Dont put anything here

    // Log initialization
    Pico::Logging::Funcs logging;
    logging.InitializeLog();    // Renames the old log file and moves if necessary
    // Endof

    // Updating the settings, calculating password hash...
    Pico::Settings::InitializeSettings();
    // Endof

    QCoreApplication a(argc, argv);

    Pico::Server::Funcs server;
    Pico::Display::Funcs display;
    Pico::Reaction::Funcs reaction;
    Pico::Input::Funcs input;
    Pico::Games::Funcs games;
    Pico::Downloader::Funcs downloader;

    display.status = "connecting...";
    display.Display();

    if (server.IsConnected()){
        display.status = "connected";
        display.Display();
    }

    while (server.IsConnected()){

        QString data = server.ReceiveData();

        ///Only waking up if the server responded with something else than an empty packet!
        if (data.length() > 0){

            /// Calling MakeResponse to :
            /// - Log in
            /// - Disconnect
            /// - Update games list

            QString response = reaction.MakeResponse(data, gamesMap, server);

            /// Checking if we're still connected...at this point, MakeResponse could have disconnected us.
            if (server.IsConnected()){

                switch (mode){

                case 0:{
                    ///
                    /// GAME LISTING
                    ///

                    if (gamesMap.size() > 0){
                        display.command = "Press ENTER to select a game";
                    }

                    int ch = getch();

                    if (ch == 10){  /// User pressed enter
                        mode = 1;   /// Going to PICK GAME mode
                        display.command = "Enter a game ID : ";
                    }

                    float currentTime = ((float)std::clock())/CLOCKS_PER_SEC;
                    if  (currentTime - lastRefresh > refreshSpeed || mode == 1){
                        display.DisplayGameList(gamesMap);
                        lastRefresh = currentTime;
                    }
                }
                    break;

                case 1:{
                        wmove(display.windows[1], 1, 1+display.command.size());
                        gamePick = input.PickGame(gamesMap, display.windows[1]);
                        if (gamePick > 0){
                            display.command = "Joining "+gamesMap[gamePick].value("title").toString().toStdString();
                            display.Display();
                            mode = 2;
                        }
                        else{
                            mode = 0;
                        }
                        break;
                    }

                case 2:{
                        if (gamePick <= 0){
                            mode = 0;
                        }
                        games.PopulateDownloadsMap(gamesMap[gamePick], downloadsMap);   /// Which files do we need to download for this game ?
                        downloader.DownloadFiles(downloadsMap);                         /// Updates downloadsMap state, download files one by one
                        float currentTime = ((float)std::clock())/CLOCKS_PER_SEC;
                        if  (currentTime - lastRefresh > refreshSpeed){
                            display.DisplayDownloads(downloadsMap);
                            lastRefresh = currentTime;
                        }
                        break;
                    }

                }

                /// Actually answering the server
                if (response.length() > 0){

                    server.SendData(response);

                }
                /// End of

            }
            else{
                display.command = " ";
                display.status = "disconnected";
                if (response.length() > 0){
                    display.status += " - " + response.toStdString();
                }
            }
        }

    };

    display.Display();

    return a.exec();
}

#include "settings.h"
#include "reaction.h"
#include "serverlink.h"
#include "logging.h"
#include "display.h"

#include <iostream>

#include <QCoreApplication>

///////////////
///
/// Initialization of vars
///
///////////////

const float refreshSpeed = 1; /// One second.

std::map<int, QJsonObject> gamesMap;
float lastRefresh = 0;


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

                ///
                /// GAME LISTING
                ///

                float currentTime = ((float)std::clock())/CLOCKS_PER_SEC;

                if  (currentTime - lastRefresh > refreshSpeed){
                    display.ListGames(gamesMap);
                    lastRefresh = currentTime;
                }

                /// END OF


                /// Actually answering the server
                if (response.length() > 0){

                    server.SendData(response);

                }
                /// End of

            }
            else{
                display.status = "disconnected";
                if (response.length() > 0){
                    display.status += " - " + response.toStdString();
                }
            }
        }

    };

    //display.Display();

    return a.exec();
}

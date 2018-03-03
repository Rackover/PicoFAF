#include "settings.h"
#include "reaction.h"
#include "serverlink.h"
#include "logging.h"
#include "display.h"

#include <QCoreApplication>

///////////////
///
/// Initialization of vars
///
///////////////

std::map<int, QJsonObject> gamesMap;


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

    while (server.IsConnected()){
        QString data = server.ReceiveData();
        display.status = "connected";

        if (data.length() > 0){
            QString response = reaction.MakeResponse(data, gamesMap);

            qDebug() << gamesMap.size();
            display.ListGames(gamesMap);

            if (response.length() > 0){

                server.SendData(response);
            }
        }

    };
    return a.exec();
}

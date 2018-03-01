#include "settings.h"
#include "reaction.h"
#include "serverlink.h"
#include "logging.h"

#include <QCoreApplication>

int main(int argc, char *argv[])
{
    //Dont put anything here

    //Log initialization
    Pico::Logging::Funcs logging;
    logging.InitializeLog();    //Renames the old log file and moves if necessary
    //Endof

    //Updating the settings, calculating password hash...
    Pico::Settings::InitializeSettings();
    //Endof


    QCoreApplication a(argc, argv);

    Pico::Server::Funcs server;
    Pico::Reaction::Funcs reaction;

    while (server.IsConnected()){
        QString data = server.ReceiveData();

        if (data.length() > 0){
            QString response = reaction.MakeResponse(data);
            if (response.length() > 0){
                server.SendData(response);
            }
        }

    };
    return a.exec();
}

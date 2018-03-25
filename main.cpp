#include "settings.h"
#include "serverlink.h"
#include "logging.h"
#include "display.h"
#include "input.h"
#include "games.h"
#include "downloader.h"

#include <iostream>

#include <QCoreApplication>
#include <QTimer>
#include <QObject>


class Mainframe : public QObject
{
    Q_OBJECT
public:
    Mainframe(QObject *parent = 0) : QObject(parent) {}

public slots:
    void Run()
    {

        ///////////////
        ///
        /// Initialization of vars
        ///
        ///////////////

        int mode = 0;   // MODE 0 : LIST GAMES. 1 : PICK GAME. 2 : JOINING GAME.
        int gamePick;   /// Used in game picking

        /// STARTUP
        // Log initialization
        Pico::Logging::Funcs logging;
        logging.InitializeLog();    // Renames the old log file and moves if necessary
        // Endof

        // Updating the settings, calculating password hash...
        Pico::Settings::InitializeSettings();
        // Endof
        /// END OF

        Pico::Server::Funcs server;
        Pico::Display::Funcs display;
        Pico::Input::Funcs input;
        Pico::Games::Funcs games;
        Pico::Downloader::Funcs downloader;


        ///////////////
        ///
        /// Connecting wires
        ///
        ///////////////

        /// - Connecting server to display
        connect(
                &server,
                SIGNAL(StatusChanged(QString)),
                &display,
                SLOT(OnStatusChange(QString)));

        connect(
                &games,
                SIGNAL(GamesChanged(std::map<int, QJsonObject>)),
                &display,
                SLOT(OnGameUpdate(std::map<int, QJsonObject>)));


        /// - Connecting server games add and remove to <Games>
        connect(
                &server,
                SIGNAL(AddGame(int, QJsonObject)),
                &games,
                SLOT(OnGameAdd(int, QJsonObject)));

        connect(
                &server,
                SIGNAL(DeleteGame(int)),
                &games,
                SLOT(OnGameDelete(int)));


        /// - Connecting pressed ENTER to modeswitch - and other keypresses
        connect(
                    &input,
                    SIGNAL(PressedEnter()),
                    this,
                    SLOT(OnPressEnter()));

        connect(
                    &input,
                    SIGNAL(KeyPress()),
                    this,
                    SLOT(OnKeyPress()));




        /// End of


        /// emit Finished();
    }

signals:
    void Finished();
};

#include "main.moc"

/*

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
        ///
        /// GAME PICKING
        ///

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
        ///
        /// GAME JOINING
        ///

        if (gamePick <= 0){
            mode = 0;
        }
        logging.Write("MAIN=>Preparing to join "+QString::number(gamePick));

        downloader.SynchronizeCustomData(gamesMap[gamePick]);   /// Which files do we need to download for this game ?

        float currentTime = ((float)std::clock())/CLOCKS_PER_SEC;
        if  (currentTime - lastRefresh > refreshSpeed){
            display.DisplayDownloads(downloader.currentDownloads);
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
}


*/






///#include "main.moc"

///////////////
///
/// Execution -- Nothing to see here
///
///////////////


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Task parented to the application so that it
    // will be deleted by the application.
    Mainframe *mainframe = new Mainframe(&a);

    // This will cause the application to exit when
    // the task signals finished.
    QObject::connect(mainframe, SIGNAL(Finished()), &a, SLOT(quit()));

    // This will run the task from the application event loop.
    QTimer::singleShot(0, mainframe, SLOT(Run()));

    return a.exec();
}


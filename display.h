#ifndef DISPLAY_H
#define DISPLAY_H

#include <wincon/pdcwin.h>
#include <panel.h>

#include <QObject>
#include <QNetworkReply>

namespace Pico{
    namespace Display{

        class Funcs : public QObject
        {
            Q_OBJECT
            public:

                WINDOW *windows[4];
                PANEL  *panels[4];


                explicit Funcs(QObject *parent = 0);

                std::list<std::string> lines;
                std::string status;
                std::string command;

                void AddLine(std::string text);

                void ResizeWindows();
                std::string BoolToCheckbox(bool value);
                void Display();
                void RefreshCommand(std::string command);
                void DisplayGameList(std::map<int, QJsonObject> *gamesMap);
                void DisplayDownloads(QVector<QNetworkReply *> currentDownloads);

        signals:

        public slots:
            void OnStatusChange(QString newStatus);
            void OnGameUpdate(std::map<int, QJsonObject> *gamesMap);

        private:

        };
    }
}

#endif // DISPLAY_H

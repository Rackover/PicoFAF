#ifndef DISPLAY_H
#define DISPLAY_H

#include <wincon/pdcwin.h>
#include <panel.h>

#include <QObject>

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

                void AddLine(std::string text);
                void Display();
                void DisplayGameList(std::map<int, QJsonObject> &gamesMap);

        signals:

        public slots:

        private:

        };
    }
}

#endif // DISPLAY_H

#ifndef INPUT_H
#define INPUT_H
#include <wincon/pdcwin.h>
#include <panel.h>

#include <QObject>

namespace Pico{
    namespace Input{

        class Funcs : public QObject
        {
            Q_OBJECT
            public:
                explicit Funcs(QObject *parent = 0);

                int PickGame(std::map<int, QJsonObject> &gamesMap, WINDOW *window);
                int PressedKey();
                char PressedChar();


            signals:

            public slots:

            private:

        };
    }
}

#endif // INPUT_H

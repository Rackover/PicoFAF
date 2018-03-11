#ifndef GAMES_H
#define GAMES_H

#include <QObject>

namespace Pico{
    namespace Games{

        class Funcs : public QObject
        {
            Q_OBJECT
            public:
                explicit Funcs(QObject *parent = 0);
        public slots:

        private:

        };
    }
}

#endif // GAMES_H

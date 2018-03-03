#ifndef REACTION_H
#define REACTION_H

#include <QObject>
#include <QString>
#include <QJsonObject>

#include <map>

namespace Pico{
    namespace Reaction{

    class Funcs : public QObject
    {
        Q_OBJECT
        public:
            explicit Funcs(QObject *parent = 0);

            QString MakeResponse(QString inputData, std::map<int, QJsonObject> &gamesMap);

            signals:

            public slots:

            private:

        };
    }
}
#endif // REACTION_H

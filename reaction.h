#ifndef REACTION_H
#define REACTION_H

#include <iostream>

#include <QObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>
#include <QTextCodec>


namespace Pico{
    namespace Reaction{

    class Funcs : public QObject
    {
        Q_OBJECT
        public:
            explicit Funcs(QObject *parent = 0);

            QString MakeResponse(QString inputData);

            signals:

            public slots:

            private:

        };
    }
}
#endif // REACTION_H

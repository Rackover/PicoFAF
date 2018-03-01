#ifndef LOGGING_H
#define LOGGING_H

#include <QObject>
#include <QString>

namespace Pico{
    namespace Logging{

    class Funcs : public QObject
    {
        Q_OBJECT
        public:
            explicit Funcs(QObject *parent = 0);

            void Write(QString inputData);
            void InitializeLog();

        signals:

        public slots:

        private:

        };
    }
}
#endif // LOGGING_H

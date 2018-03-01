#ifndef SOCKETTEST_H
#define SOCKETTEST_H

#include <QString>
#include <QTcpSocket>

namespace Pico{
    namespace Server{

    class Funcs : public QObject
    {
        Q_OBJECT
        public:
            explicit Funcs(QObject *parent = 0);

            QTcpSocket socket;

            bool IsConnected();
            QString ReceiveData();
            void CloseConnection();
            void SendData(QString data);

        signals:

        public slots:

        private:

        };
    }
}

#endif // SOCKETTEST_H

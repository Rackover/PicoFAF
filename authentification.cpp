#include "authentification.h"

#include <QString>
#include <QProcess>
#include <QDir>
#include <QTextCodec>

namespace Pico{
    namespace Auth{
        QString GetUid(double sessionId){
            QString uid = "-1";

            QProcess* process = new QProcess();
            QString file = QDir::currentPath() + "/EXTERNAL/UIDGENRT.exe " + QString::number(sessionId, 'f', 0);
            QString output;
            process->start(file);

            while (!process->waitForFinished(4000)){

            }
            uid = QTextCodec::codecForMib(106)->toUnicode(process->readAll());

            return uid;
        }
    }
}

#include "authentificationManager.h"

QString SocketManager::GetUid(double sessionId){
    QString uid = "-1";

    QProcess *process = new QProcess(this);
    QString file = QDir::currentPath() + "/EXTERNAL/UIDGENRT.exe " + QString::number(sessionId, 'f', 0);
    QString output;
    process->start(file);

    while (!process->waitForFinished(4000)){

    }
    uid = QTextCodec::codecForMib(106)->toUnicode(process->readAll());

    return uid;
}
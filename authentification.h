#ifndef AUTHENTIFICATION_H
#define AUTHENTIFICATION_H

#include <QString>
#include <QProcess>
#include <QDir>
#include <QTextCodec>

namespace Pico{
    namespace Auth{
        QString GetUid(double sessionId);
    }
}
#endif // AUTHENTIFICATION_H

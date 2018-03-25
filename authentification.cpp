#include "authentification.h"
#include "utils.h"
#include "settings.h"
#include "logging.h"

#include <QDebug>
#include <QString>
#include <QProcess>
#include <QDir>
#include <QTextCodec>

namespace Pico{
    namespace Auth{
        QString GetUid(double sessionId){

            Pico::Logging::Funcs logging;

            logging.Write("GET_UID => Called for an UID generation");
            QString uid = "-1";

            QProcess* process = new QProcess();
            QString file = QDir::currentPath() + "/" + QString::fromStdString(Pico::Settings::HC_CONFIG.UID_EXE_PATH) + " " + QString::number(sessionId, 'f', 0);

            logging.Write("GET_UID => Looking for "+file);

            if (!Pico::Utils::file_exists(Pico::Settings::HC_CONFIG.UID_EXE_PATH)){
                logging.Write("GET_UID => UID Generator couldn't be find. Exiting.");
                return uid;
            }

            logging.Write("GET_UID => Found it.");

            QString output;
            process->start(file);


            while (!process->waitForFinished(3000)){
                if (process->error() < 5){
                    logging.Write("GET_UID => Could not generate UID : "+process->errorString()+". Exiting.");
                    return uid;
                }
            }
            logging.Write("GET_UID => Process finished");
            uid = QTextCodec::codecForMib(106)->toUnicode(process->readAll());

            return uid;
        }
    }
}

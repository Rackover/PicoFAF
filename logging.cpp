#include "logging.h"
#include "settings.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <QDateTime>
#include <QDir>
#include <QObject>
#include <QString>

namespace Pico{
    namespace Logging{

        Funcs::Funcs(QObject *parent) :
            QObject(parent)
        {
        }

        Pico::Settings::FD_CONFIG FDC;
        std::clock_t startClock;

        void Funcs::Write(QString message){
            QDir logDirectory;
            if (logDirectory.mkpath(QString::fromStdString(FDC.LOG_PATH))){

                QString completePath = QString::fromStdString(FDC.LOG_PATH+Pico::Settings::HC_CONFIG.LOG_FILE);

                std::ofstream logStream;
                logStream.open(completePath.toStdString(), std::ofstream::out | std::ofstream::app);

                std::ostringstream timePassedStream;
                timePassedStream << ((float)std::clock() - (float)startClock);
                std::string timePassed(timePassedStream.str());

                logStream << timePassed << " -> " << message.toStdString() << std::endl;
                logStream.close();

            }
            else{

                std::cout << "[ERR] UNABLE TO LOCATE OR CREATE LOG FILE !" << std::endl;
                std::cout << "[ERR] FUTURE ERRORS WILL NOT BE LOGGED" << std::endl;

            }

        }

        void Funcs::InitializeLog(){
            QString completePath = QString::fromStdString(FDC.LOG_PATH+Pico::Settings::HC_CONFIG.LOG_FILE);

            QFile oldLog (completePath);
            if (oldLog.exists()){
                QFileInfo oldLogInfo (oldLog);
                oldLog.rename(oldLog.fileName(), oldLog.fileName()+"."+oldLogInfo.lastModified().toString("yyyy.MM.dd.HH.mm.ss"));
            }

            Write("FROM INITIALIZE_LOG => LOG START");
        }
    }
}

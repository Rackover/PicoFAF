#include "logging.h"
#include "settings.h"

namespace Pico{
    namespace Logging{

        Funcs::Funcs(QObject *parent) :
            QObject(parent)
        {
        }

        FD_CONFIG FDC;
        std::clock_t startClock;

        void Funcs::Write(QString message){
            QDir logDirectory;
            if (logDirectory.mkpath(QString::fromStdString(FDC.LOGPATH))){

                QString completePath = QString::fromStdString(FDC.LOGPATH+HC_CONFIG.LOGFILE);

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
            QString completePath = QString::fromStdString(FDC.LOGPATH+HC_CONFIG.LOGFILE);

            QFile oldLog (completePath);
            if (oldLog.exists()){
                QFileInfo oldLogInfo (oldLog);
                oldLog.rename(oldLog.fileName(), oldLog.fileName()+"."+oldLogInfo.lastModified().toString("yyyy.MM.dd.HH.mm.ss"));
            }

            Write("FROM INITIALIZE_LOG => LOG START");
        }
    }
}

    /*
    void LogToFile(QtMsgType type, const char *msg) {


      QString datetime = QDateTime::currentDateTime().toStdString("yyyy.MM.dd hh:mm:ss");
      switch (type) {
        case QtDebugMsg:
          logStream << datetime.toStdString().data() << " [Debug] " << msg << "\n";
          break;
        case QtCriticalMsg:
          logStream << datetime.toStdString().data() << " [Critical] " << msg << "\n";
          break;
        case QtWarningMsg:
          logStream << datetime.toStdString().data() << " [Warning] " << msg << "\n";
          break;
        case QtFatalMsg:
          logStream << datetime.toStdString().data() <<  " [Fatal] " << msg << "\n";
          abort();
      }


    }

    qInstallMessageHandler(LogToFile);
    */

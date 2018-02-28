#include <fstream>
#include <ostream>
#include <sstream>

#include "logManager.h"
#include "settings.h"


namespace LogManager {

    FD_CONFIG FDC;

    void LogManager::Write(std::string message){

        std::ofstream logStream;
        logStream.open(FDC.LOGFILE, std::ofstream::out | std::ofstream::app);

        std::ostringstream timePassedStream;
        timePassedStream << ((float)std::clock() - (float)startClock);
        std::string timePassed(timePassedStream.str());

        logStream << timePassed << " -> " << message;
        logStream.close();
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
}

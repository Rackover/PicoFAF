#include "logManager.h"
#include "settings.h"

namespace LogManager {

LogManager::LogManager(QObject *parent) :
    QObject(parent)
{
}
    FD_CONFIG FDC;

    static const void LogManager::Write(std::string data){
        std::ofstream fl;
        fl.open(FDC.LOGFILE);
        fl << data;
        fl.close();
    }

}

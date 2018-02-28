#include <QCoreApplication>

#include "responseManager.h"
#include "socketManager.h"
#include "logManager.h"

int main(int argc, char *argv[])
{
    //Dont put anything above Log initialization
    LogManager::LogManager logApp;
    logApp.InitializeLog();    //Renames the old log file and moves if necessary


    QCoreApplication a(argc, argv);

    SocketManager::SocketManager socketApp;
    ResponseManager::ResponseManager responseApp;

    while (socketApp.IsConnected()){
        QString data = socketApp.ReceiveData();

        if (data.length() > 0){
            QString response = responseApp.MakeResponse(data);
            if (response.length() > 0){
                socketApp.SendData(response);
            }
        }

    };
    return a.exec();
}

#include "main.h"

#include "responseManager.h"
#include "socketManager.h"

#include "utils.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


    Utils utils;
    SocketManager socketApp;
    ResponseManager responseApp;

    QTcpSocket* currentSocket = socketApp.InitalizeConnection();

    while (socketApp.IsConnected(currentSocket)){
        QString data = socketApp.ReceiveData(currentSocket);

        if (data.length() > 0){
            QString response = responseApp.MakeResponse(data);
            if (response.length() > 0){
                socketApp.SendData(currentSocket, response);
            }
        }

    };
    return a.exec();
}

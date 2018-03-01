#ifndef SETTINGS_H
#define SETTINGS_H

#include <QCryptographicHash>
#include <string>
#include <fstream>
#include <streambuf>

namespace Pico{
    namespace Settings{
        static const struct HC_CONFIG{
            std::string CLIENT_VERSION =    "0.13.2";
            std::string LOG_FILE =          "PICOCLNT.LOG";
            std::string SETTINGS_FILE =     "CONFIG.TXT";
        } HC_CONFIG;


        struct FD_CONFIG{
            std::string SERVER_ADDRESS =    SERVER_ADDRESS_INIT;
            int         SERVER_PORT =       SERVER_PORT_INIT;
            std::string LOGIN =             LOGIN_INIT;
            std::string PASSWORD =          PASSWORD_INIT;
            std::string HASHWORD =          HASHWORD_INIT;
            std::string LOG_PATH =          LOG_PATH_INIT;

            static std::string   SERVER_ADDRESS_INIT;
            static int           SERVER_PORT_INIT;
            static std::string   LOGIN_INIT;
            static std::string   PASSWORD_INIT;
            static std::string   HASHWORD_INIT;
            static std::string   LOG_PATH_INIT;
        };

        void InitializeSettings();

    }
}

/*
/////////////////////
///
///  DEBUG ONLY - TO CHANGE
///
static const struct DEBUG_FD_CONFIG{
    std::string SERVER_ADDRESS = "test.faforever.com";
    int SERVER_PORT = 8001;
    std::string LOGIN = "Rackover";
    std::string PASSWORD = "foo";
    std::string HASHWORD = "fooHash";
    std::string LOGFILE = "LOGS/PICOCLNT.LOG";
} DEBUG_FD_CONFIG;
///
/// END OF
///////////////////
*/
#endif // SETTINGS_H

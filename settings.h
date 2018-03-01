#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>

namespace Pico{
    namespace Settings{
        static const struct HC_CONFIG{
            std::string CLIENT_VERSION =    "0.13.2";
            std::string LOG_FILE =          "PICOCLNT.LOG";
            std::string SETTINGS_FILE =     "CONFIG.TXT";
        } HC_CONFIG;


        struct FD_CONFIG{
            static std::string   SERVER_ADDRESS;
            static int           SERVER_PORT;
            static std::string   LOGIN;
            static std::string   PASSWORD;
            static std::string   HASHWORD;
            static std::string   LOG_PATH;
        };

        void InitializeSettings();
        void CreateConfigFile();
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

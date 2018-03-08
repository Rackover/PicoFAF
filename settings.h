#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>

namespace Pico{
    namespace Settings{
        static const struct HC_CONFIG{
            std::string CLIENT_VERSION =    "0.13.2";
            std::string LOG_FILE =          "pico-client.log";
            std::string SETTINGS_FILE =     "config.txt";
            std::string UID_EXE_PATH =      "external/uid-generator.exe";
            std::string CLIENT_NAME =       "Pico Client";
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

#endif // SETTINGS_H

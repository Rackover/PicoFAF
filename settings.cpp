#include "settings.h"

#include "utils.h"
// ^ Defined here to avoid definition loop

namespace Pico{
    namespace Settings{

        FD_CONFIG _FDC;

         std::string   _FDC.SERVER_ADDRESS_INIT =   "lobby.faforever.com";
         int           _FDC.SERVER_PORT_INIT =      8001;
         std::string   _FDC.LOGIN_INIT =            "Player";
         std::string   _FDC.PASSWORD_INIT =         "foo";
         std::string   _FDC.HASHWORD_INIT =         "";
         std::string   _FDC.LOG_PATH_INIT =         "LOGS/";


        void InitializeSettings(){

            FD_CONFIG FDC;



            std::string configPath = HC_CONFIG.SETTINGS_FILE;

            std::ifstream t(configPath);
            std::string settingsContent((std::istreambuf_iterator<char>(t)),
                             std::istreambuf_iterator<char>());

            std::vector<std::string> settingsLines = Pico::Utils::explode(settingsContent, '\n');

            for (int i = 0; i < (int)settingsLines.size(); i++){

                std::string thisLine = settingsLines.at(i);
                if (thisLine.find(":") == std::string::npos){
                    continue;
                }

                std::vector<std::string> lineContents = Pico::Utils::explode(thisLine, ':');

                std::string settingName = lineContents.at(0);
                std::string settingValue = lineContents.at(1);

                switch (Pico::Utils::str2int(settingName.data())){

                    case Pico::Utils::str2int("SERVER_ADDRESS"):{
                        std::string FDC.SERVER_ADDRESS_INIT = settingValue;
                        break;
                    }

                }
            }
        }
    }
}
/*
                std::string SERVER_ADDRESS = "lobby.faforever.com";
                int SERVER_PORT = 8001;
                std::string LOGIN = "Rackover";
                std::string PASSWORD = "foo";
                std::string HASHWORD = "fooHash";
                std::string LOG_PATH = "LOGS/";
*/

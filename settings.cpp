#include "settings.h"

#include "logging.h"
#include "utils.h"
// ^ Defined here to avoid definition loop

namespace Pico{
    namespace Settings{

        Pico::Logging::Funcs logging;

         std::string   FD_CONFIG::SERVER_ADDRESS_INIT =   "lobby.faforever.com";
         int           FD_CONFIG::SERVER_PORT_INIT =      8001;
         std::string   FD_CONFIG::LOGIN_INIT =            "Player";
         std::string   FD_CONFIG::PASSWORD_INIT =         "foo";    ///MUST NEVER BE OF ZERO LENGTH IN THE INITIALIZATION
         std::string   FD_CONFIG::HASHWORD_INIT =         "";
         std::string   FD_CONFIG::LOG_PATH_INIT =         "LOGS/";


        void InitializeSettings(){

            logging.Write("From INITIALIZE_CONFIG => Starting config initialization.");

            std::ostringstream newSettingsStream;

            std::string configPath = HC_CONFIG.SETTINGS_FILE;

            std::ifstream t(configPath);
            std::string settingsContent((std::istreambuf_iterator<char>(t)),
                             std::istreambuf_iterator<char>());
            std::cout << "TEST " << configPath;
            std::vector<std::string> settingsLines = Pico::Utils::explode(settingsContent, '\n');

            for (int i = 0; i < (int)settingsLines.size(); i++){

                std::string thisLine = settingsLines.at(i);
                if (thisLine.find(":") == std::string::npos){
                    continue;
                }

                std::vector<std::string> lineContents = Pico::Utils::explode(thisLine, ':');

                std::string settingName = lineContents.at(0);
                std::string settingValue = lineContents.at(1);

                bool readData = true;

                switch (Pico::Utils::str2int(settingName.data())){

                    default:{
                        readData = false;
                        break;
                    }

                    case Pico::Utils::str2int("SERVER_ADDRESS"):{
                        FD_CONFIG::SERVER_ADDRESS_INIT = settingValue;
                        break;
                    }
                    case Pico::Utils::str2int("SERVER_PORT"):{
                        FD_CONFIG::SERVER_PORT_INIT = std::stoi(settingValue);
                        break;
                    }

                    case Pico::Utils::str2int("LOGIN"):{
                        FD_CONFIG::LOGIN_INIT = settingValue;
                        break;
                    }

                    case Pico::Utils::str2int("PASSWORD"):{
                        ///
                        ///INIT PASSWORD
                        ///
                        if (settingValue.length() > 0){

                            QByteArray hashwordBA = QCryptographicHash::hash(QString::fromStdString(settingValue).toUtf8(), QCryptographicHash::Sha256);
                            QString hashword = QString::fromUtf8(hashwordBA);

                            FD_CONFIG::PASSWORD_INIT = "";
                            FD_CONFIG::HASHWORD_INIT = hashword.toStdString();
                        }

                        break;
                    }

                    case Pico::Utils::str2int("HASHWORD"):{
                        readData = false;
                        /// If PASSWORD_INIT is "", then hashword is fresh. No need to reassign it from config file.
                        if (FD_CONFIG::PASSWORD_INIT.length() > 0){
                            FD_CONFIG::HASHWORD_INIT = settingValue;
                            readData = true;
                        }
                        break;
                    }

                    case Pico::Utils::str2int("LOG_PATH"):{
                        FD_CONFIG::LOG_PATH_INIT = settingValue;
                        break;
                    }
                }
                if (readData){
                    logging.Write("From INITIALIZE_CONFIG => READ "+QString::fromStdString(settingName)+" ["+QString::fromStdString(settingValue)+"]");
                    newSettingsStream << settingName << ":" << settingValue << std::endl;
                }
            }
            std::string newSettings(newSettingsStream.str());
            logging.Write("From INITIALIZE_CONFIG => Finished config initialization. Current config is : \""+QString::fromStdString(newSettings)+"\"");

            QFile oldSettingsFile(QString::fromStdString(configPath));
            oldSettingsFile.remove();


            QFile newSettingsFile (QString::fromStdString(configPath));
            if (newSettingsFile.open(QIODevice::ReadWrite) )
            {
                QTextStream stream( &newSettingsFile );
                stream << QString::fromStdString(newSettings);
                newSettingsFile.close();
                logging.Write("From INITIALIZE_CONFIG => Successfully wrote to disk.");
            }
        }
    }
}

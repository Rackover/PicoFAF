#include "settings.h"

#include "logging.h"
#include "utils.h"

#include <iostream>
#include <fstream>
#include <map>
#include <QDebug>
#include <QFile>

namespace Pico{
    namespace Settings{

        Pico::Logging::Funcs logging;

         std::string   FD_CONFIG::SERVER_ADDRESS =   "lobby.faforever.com";
         int           FD_CONFIG::SERVER_PORT =      8001;
         std::string   FD_CONFIG::LOGIN =            "Player";
         std::string   FD_CONFIG::PASSWORD =         "foo";
         std::string   FD_CONFIG::HASHWORD =         "";
         std::string   FD_CONFIG::LOG_PATH =         "LOGS/";


        void InitializeSettings(){

            logging.Write("From INITIALIZE_CONFIG => Starting config initialization.");

            std::ostringstream newSettingsStream;

            std::string configPath = HC_CONFIG.SETTINGS_FILE;

            std::ifstream t(configPath);
            std::string settingsContent((std::istreambuf_iterator<char>(t)),
                             std::istreambuf_iterator<char>());

            bool exists = Utils::file_exists(configPath);
            if (!exists){
                CreateConfigFile();
            }

            std::vector<std::string> settingsLines = Pico::Utils::explode(settingsContent, '\n');

            for (int i = 0; i < (int)settingsLines.size(); i++){

                std::string thisLine = settingsLines.at(i);
                if (thisLine.find(":") == std::string::npos){
                    continue;
                }

                std::vector<std::string> lineContents = Pico::Utils::explode(thisLine, ':');

                std::string settingName = lineContents.at(0);
                std::string settingValue = "";
                if (lineContents.size() > 1){
                    settingValue = lineContents.at(1);
                }

                bool readData = true;

                switch (Pico::Utils::str2int(settingName.data())){

                    default:{
                        readData = false;
                        break;
                    }

                    case Pico::Utils::str2int("SERVER_ADDRESS"):{
                        FD_CONFIG::SERVER_ADDRESS = settingValue;
                        break;
                    }
                    case Pico::Utils::str2int("SERVER_PORT"):{
                        FD_CONFIG::SERVER_PORT = std::stoi(settingValue);
                        break;
                    }

                    case Pico::Utils::str2int("LOGIN"):{
                        FD_CONFIG::LOGIN = settingValue;
                        break;
                    }

                    case Pico::Utils::str2int("PASSWORD"):{
                        if (settingValue.length() > 0){
                            QByteArray hashwordBA = QCryptographicHash::hash(QByteArray::fromStdString(settingValue),QCryptographicHash::Sha256).toHex();
                            QString hashword = QString(hashwordBA);
                            settingValue = "";
                            FD_CONFIG::HASHWORD = hashword.toStdString();
                        }

                        break;
                    }

                    case Pico::Utils::str2int("HASHWORD"):{
                        if (FD_CONFIG::HASHWORD.length() == 0){
                            FD_CONFIG::HASHWORD = settingValue;
                        }
                        settingValue = FD_CONFIG::HASHWORD;
                        break;
                    }

                    case Pico::Utils::str2int("LOG_PATH"):{
                        FD_CONFIG::LOG_PATH = settingValue;
                        break;
                    }
                }
                if (readData){
                    logging.Write("From INITIALIZE_CONFIG => SET "+QString::fromStdString(settingName)+" TO ["+QString::fromStdString(settingValue)+"]");
                    newSettingsStream << settingName << ":" << settingValue << std::endl;
                }
            }
            std::string newSettings(newSettingsStream.str());
            logging.Write("From INITIALIZE_CONFIG => Finished config initialization.");

            QFile oldSettingsFile(QString::fromStdString(configPath));
            oldSettingsFile.remove();


            QFile newSettingsFile (QString::fromStdString(configPath));
            if (newSettingsFile.open(QIODevice::ReadWrite) )
            {
                QTextStream stream(&newSettingsFile);
                stream << QString::fromStdString(newSettings);
                newSettingsFile.close();
                logging.Write("From INITIALIZE_CONFIG => Successfully wrote to disk.");
            }
        }

        void CreateConfigFile(){
/*
            std::string configPath = HC_CONFIG.SETTINGS_FILE;
            QFile newSettingsFile (QString::fromStdString(configPath));

            if (newSettingsFile.open(QIODevice::WriteOnly) )
            {

                QTextStream stream( &newSettingsFile);
                //std::vector<std::vector<QString, QString>> settingsList = {{"SERVER_ADDRESS", FD_CONFIG::SERVER_ADDRESS}};
                std::map<QString, QString> defaultSettings;
                defaultSettings.insert(std::pair<QString, QString>("SERVER_ADDRESS", FD_CONFIG::SERVER_ADDRESS));

                for (const auto& pair : defaultSettings) {
                    stream << pair.first << ":" << pair.second << std::endl;
                }

                newSettingsFile.close();
                logging.Write("From CREATE_CONFIG_FILE => Created a new config file.");
            }
        */
        }
    }
}

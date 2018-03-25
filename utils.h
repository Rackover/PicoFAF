#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <sstream>
#include <map>

#include <QCryptographicHash>
#include <QString>

namespace Pico{
    namespace Utils{
        inline std::vector<std::string> explode(std::string const & s, char delim)
        {
            std::vector<std::string> result;
            std::istringstream iss(s);

            for (std::string token; std::getline(iss, token, delim); )
            {
                result.push_back(std::move(token));
            }

            return result;
        }

        inline std::vector<std::string> splitOnFirst(std::string const & s, char delim)
        {
            std::vector<std::string> result;
            std::istringstream iss(s);

            for (std::string token; std::getline(iss, token, delim); )
            {
                result.push_back(std::move(token));
                break;
            }

            return result;
        }

        constexpr unsigned int str2int(const char* str, int h = 0)
        {
            return !str[h] ? 5381 : (str2int(str, h+1) * 33) ^ str[h];
        }

        inline bool file_exists (const std::string& name) {
            if (FILE *file = fopen(name.c_str(), "r")) {
                fclose(file);
                return true;
            } else {
                return false;
            }
        }

        inline QString sha256 (QString data){
            data = QString("%1").arg(QString(QCryptographicHash::hash(data.toUtf8(),QCryptographicHash::Md5).toHex()));
            return data;
        }

        inline std::string makeLong (std::string text, int length){
            text = text.substr(0, length);
            for (int i = text.size(); i < length; i++){
                text += " ";
            }
            return text;
        }

        inline std::string map_to_string(std::map<std::string, int>  &m) {
            std::string output = "";
            std::string convrt = "";
            std::string result = "";

            for (auto it = m.cbegin(); it != m.cend(); it++) {

                convrt = std::to_string(it->second);
                output += (it->first) + ":" + (convrt) + ", ";
            }

            result = output.substr(0, output.size() - 2 );

          return result;
        }
    }
}

#endif // UTILS_H

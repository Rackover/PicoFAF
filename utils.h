#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <sstream>
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
    }
}

#endif // UTILS_H

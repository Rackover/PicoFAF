#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <sstream>
#include <utility>

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
    }
}

#endif // UTILS_H

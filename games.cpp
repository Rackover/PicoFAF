#include "games.h"

namespace Pico{
    namespace Games{

        Funcs::Funcs(QObject* parent) :
            QObject(parent)
        {
        }

        void Funcs::PopulateDownloadsMap(QJsonObject game, std::map<std::string, int> downloadsMap){

        }
    }
}

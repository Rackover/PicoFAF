#ifndef SETTINGS_H
#define SETTINGS_H

#include <QCryptographicHash>

static const struct HC_CONFIG{
    std::string CLIENT_VERSION = "0.13.2";
} HC_CONFIG;


struct FD_CONFIG{
    std::string SERVER_ADDRESS = "test.faforever.com";
    int SERVER_PORT = 8001;
    std::string LOGIN = "Rackover";
    std::string PASSWORD = "foo";
    std::string HASHWORD = "fooHash";
    std::string LOGFILE = "LOGS/PICOCLNT.LOG";
};


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

#endif // SETTINGS_H

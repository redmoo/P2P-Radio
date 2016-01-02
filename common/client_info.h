#ifndef CLIENT_INFO
#define CLIENT_INFO

#include <QtNetwork>
#include "common_global.h"


struct COMMONSHARED_EXPORT client_info{
    QHostAddress addres;
    quint16 port;
};

#endif // CLIENT_INFO


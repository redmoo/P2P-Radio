#ifndef CLIENTINFO_H
#define CLIENTINFO_H

//#include "common_global.h"
#include <QtNetwork>

class ClientInfo
{

public:
    ClientInfo(QHostAddress, quint16);
    QHostAddress address;
    quint16 port;

}; // TODO: struct?

#endif // CLIENTINFO_H

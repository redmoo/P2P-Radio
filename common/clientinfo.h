#ifndef CLIENTINFO_H
#define CLIENTINFO_H

#include <QtNetwork>

class ClientInfo
{
public:
    ClientInfo(QHostAddress, quint16);
    QHostAddress address;
    quint16 port;
};

#endif // CLIENTINFO_H

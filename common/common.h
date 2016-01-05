#ifndef COMMON_H
#define COMMON_H

#include "common_global.h"

#include <QAudioFormat>
#include <QtNetwork>


class COMMONSHARED_EXPORT Common
{

public:

    struct ClientInfo
    {
        ClientInfo(QTcpSocket *c) : connection(c)
        {
            ID = c->socketDescriptor();
        }

        QTcpSocket *connection;
        QHostAddress address;
        quint16 port;
        int ID;
    };

    struct DataPacket // alignment?
    {
        /*DataPacket(char d[], unsigned int id) : ID(id)
        {
            data = d;
        }*/

        char data[512];
        unsigned int ID;
    };

    static QAudioFormat getFormat();

};

#endif // COMMON_H

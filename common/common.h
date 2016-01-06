#ifndef COMMON_H
#define COMMON_H

#include "common_global.h"

#include <QAudioFormat>
#include <QtNetwork>


class COMMONSHARED_EXPORT Common
{

public:

    /**** INSTRUCTIONS ****/

    enum CommandID: quint8
    {
        MESSAGE,
        DESTINATION
    };

    struct MessageCommand // TODO: verjetno se ClientInfo sem notr zapakira??
    {
        MessageCommand() : cid(MESSAGE) {}
        MessageCommand(QString msg) : message(msg), cid(MESSAGE) {}

        QByteArray serialize()
        {
            QByteArray byteArray;

            QDataStream stream(&byteArray, QIODevice::WriteOnly);
            stream.setVersion(QDataStream::Qt_5_0);

            stream << cid
                   << message;

            return byteArray;
        }

        MessageCommand* deserialize(const QByteArray& byteArray)
        {
            QDataStream stream(byteArray);
            stream.setVersion(QDataStream::Qt_5_0);

            stream >> cid
                   >> message;

            return this;
        }

        quint8 cid;
        QString message;
    };

    /**** DATA PACKETS ****/

    //#pragma pack(push, 1)
    struct ClientInfo
    {
        ClientInfo(QTcpSocket *c) : connection(c)
        {
            ID = c->socketDescriptor();
        }


        ClientInfo(QTcpSocket *c, QHostAddress addr, qint16 port) : connection(c), address(addr), port(port)
        {
            ID = c->socketDescriptor();
        }

        QByteArray serialize()
        {
            QByteArray byteArray;

            QDataStream stream(&byteArray, QIODevice::WriteOnly);
            stream.setVersion(QDataStream::Qt_5_0);

            stream << ID
                   << address
                   << port;

            return byteArray;
        }

        void deserialize(const QByteArray& byteArray)
        {
            QDataStream stream(byteArray);
            stream.setVersion(QDataStream::Qt_5_0);

            stream >> ID
                   >> address
                   >> port;
        }

        int ID;

        QTcpSocket *connection;
        QHostAddress address;
        quint16 port;
    };
    //#pragma pack(pop)

    struct DataPacket // alignment?
    {
        DataPacket(unsigned int id, QByteArray qba) : ID(id), data(qba) {}

        QByteArray serialize()
        {
            QByteArray byteArray;

            QDataStream stream(&byteArray, QIODevice::WriteOnly);
            stream.setVersion(QDataStream::Qt_5_0);

            stream << ID
                   << data;

            return byteArray;
        }

        void deserialize(const QByteArray& byteArray)
        {
            QDataStream stream(byteArray);
            stream.setVersion(QDataStream::Qt_5_0);

            stream >> ID
                   >> data;
        }

        unsigned int ID;
        QByteArray data;
    };

    static QAudioFormat getFormat();

};

#endif // COMMON_H

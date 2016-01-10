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
        MESSAGE = 2,
        STREAM = 3
    };

    struct MessageCommand
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

            cid = MESSAGE;
            stream >> message;

            return this;
        }

        quint8 cid;
        QString message;
    };

    struct StreamCommand
    {
        StreamCommand() {}

        StreamCommand(bool reset)
            : address("127.0.0.1")
            , port(0)
            , reset_destinations(reset)
            , cid(STREAM)
        {}

        StreamCommand(QString add, quint16 p, bool reset = true)
            : address(add)
            , port(p)
            , reset_destinations(reset)
            , cid(STREAM)
        {}

        QByteArray serialize()
        {
            QByteArray byteArray;

            QDataStream stream(&byteArray, QIODevice::WriteOnly);
            stream.setVersion(QDataStream::Qt_5_0);

            stream << cid
                   << address
                   << port
                   << reset_destinations;

            return byteArray;
        }

        void deserialize(const QByteArray& byteArray, bool all = false)
        {
            QDataStream stream(byteArray);
            stream.setVersion(QDataStream::Qt_5_0);

            if (!all) cid = STREAM;
            else stream >> cid;

            stream >> address
                   >> port
                   >> reset_destinations;
        }

        quint8 cid;
        bool reset_destinations;

        QString address;
        quint16 port;
    };

    /**** DATA PACKETS ****/

    //#pragma pack(push, 1)
    struct ClientInfo
    {
        ClientInfo() : address(QHostAddress::LocalHost), port(0), ID(-1) {}

        ClientInfo(QHostAddress addr, quint16 port) : address(addr), port(port), ID(-1) {}

        ClientInfo(QTcpSocket *c, QHostAddress addr, quint16 port) : connection(c), address(addr), port(port)
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

        ClientInfo* deserialize(const QByteArray& byteArray)
        {
            QDataStream stream(byteArray);
            stream.setVersion(QDataStream::Qt_5_0);

            stream >> ID
                   >> address
                   >> port;

            return this;
        }

        int ID;

        QTcpSocket *connection;
        QHostAddress address; // not redundant. needed for sending to other clients
        quint16 port; // UDP port
    };
    //#pragma pack(pop)

    struct DataPacket
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

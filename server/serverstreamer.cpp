#include "serverstreamer.h"

#include <iostream>
#include <QTime>
#include <QtTest/QTest>


ServerStreamer::ServerStreamer(QObject *parent) : QObject(parent) {}

void ServerStreamer::init(QString ip, QString port)
{
    serverUdpSocket = new QUdpSocket(this);
    tcpServer = new QTcpServer(this);

    // Port = 0 means that the port is selected automatically.
    if (!tcpServer->listen(ip.isEmpty() ? QHostAddress::Any : QHostAddress(ip),
                           port.isEmpty() ? 0 : port.toUInt()))
    {
        qDebug () << tcpServer->errorString();
        tcpServer->close();
        return;
    }

    if (ip.isEmpty())
    {
        QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
        // use the first non-localhost IPv4 address
        for (int i = 0; i < ipAddressesList.size(); ++i)
        {
            if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
               ipAddressesList.at(i).toIPv4Address())
            {
                ip = ipAddressesList.at(i).toString();
                break;
            }
        }
        // if we did not find one, use IPv4 localhost
        if (ip.isEmpty())
            ip = QHostAddress(QHostAddress::LocalHost).toString();
    }

    connect(tcpServer, &QTcpServer::newConnection, this, &ServerStreamer::clientConnected);

    player = new Player(music_file);
    connect(player->source, &AudioSource::dataReady, this, &ServerStreamer::write);

    emit(connectionInfoChanged(ip, tcpServer->serverPort()));
}

void ServerStreamer::startStream(QString ip, QString port, bool chain)
{
    chain_streaming = chain;
    init(ip, port);
}

void ServerStreamer::setMusic(QString file)
{
    music_file = file;
}

void ServerStreamer::clientConnected()
{
    qint16 blockSize = 0;

    QTcpSocket *clientConnection = tcpServer->nextPendingConnection();
    qDebug() << "Local addr/port:" << clientConnection->localAddress() << clientConnection->localPort();
    qDebug() << "Peer addr/port:" << clientConnection->peerAddress() << clientConnection->peerPort() << clientConnection->peerName();

    if(clientConnection->waitForReadyRead())
    {
        QDataStream in(clientConnection);
        in.setVersion(QDataStream::Qt_5_0);

        in >> blockSize;

        quint16 clientPort = 0;
        in >> clientPort;

        if (clientPort == 0)
        {
            std::cerr << "Client data took too long to arrive!" << std::endl;
            return;
        }

        qDebug() << "Client sent its info. UDP port: " + QString::number(clientPort);

        auto *new_client = new Common::ClientInfo(clientConnection, clientConnection->peerAddress(), clientPort);

        if (chain_streaming && !clients.isEmpty())
            sendStreamInstruction(clients.last(), new_client);

        QVector<Common::ClientInfo *> rcps;
        rcps.append(new_client);
        sendMessage(player->currentlyPlaying(), rcps);

        clients << new_client;

        connect(clientConnection, &QAbstractSocket::disconnected, clientConnection, &QAbstractSocket::deleteLater);
        connect(clientConnection, &QAbstractSocket::disconnected, this, &ServerStreamer::clientDisconnected);

        emit clientCountChanged(clients.length());
    }
}

void ServerStreamer::clientDisconnected()
{
    // When a client disconnects, there are 3 possible outcomes.
    // 1.: First client disconnected. Server starts sending data to the second (new first) client.
    // 2.: Last client disconnected. Second to last (new last) client needs to stop sending data.
    // 3.: Client in the middle of two other clients disconnected.
    //     Previous client in the chain needs to send data to the next client in the chain.

    QMutableVectorIterator<Common::ClientInfo *> iterator(clients);

    while(iterator.hasNext())
    {
        auto p = iterator.hasPrevious() ? iterator.peekPrevious() : NULL;
        auto c = iterator.next();
        qDebug() << "DC:" << c->connection->socketDescriptor() << c->connection->isOpen() << c->connection->state();

        if (c->connection->state() == QAbstractSocket::UnconnectedState)
        {
            if (chain_streaming && p != NULL && iterator.hasNext())
            {
                qDebug() << "Connecting previous and next client.";
                sendStreamInstruction(p, iterator.peekNext());
            }
            else if (chain_streaming && p != NULL)
            {
                qDebug() << "Last client to stop sending chain packets.";
                sendStreamInstruction(p, new Common::ClientInfo());
            }

            c->connection->close();
            iterator.remove();
        }
    }

    emit clientCountChanged(clients.length());
}

void ServerStreamer::write(const QVector<QByteArray> data)
{
    foreach (auto chunk, data)
    {
        for (int i = 0; i < clients.length(); i++)
        {
            if (chain_streaming && i > 0) return;

            quint64 bytes_sent = serverUdpSocket->writeDatagram(chunk, chunk.size(), clients[i]->address, clients[i]->port);

            if(bytes_sent == -1)
                qDebug() << "CHUNK TOO BIG! (ServerStreamer::write)";
        }
    }
}

void ServerStreamer::datagramSent()
{
    qDebug() << "Data packet sent.";
}

void ServerStreamer::sendMessage(const QString msg, const QVector<Common::ClientInfo *> dsts)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion (QDataStream::Qt_5_0);

    QString message = msg.isEmpty() ? QString("Ping! Time: %1").arg(QTime::currentTime().toString()) : msg;

    out << (quint16)0;
    block.append(Common::MessageCommand(message).serialize());
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));

    foreach (Common::ClientInfo *c, dsts.isEmpty() ? clients : dsts)
        c->connection->write(block);

    qDebug() << "Server: Ping message sent.";
}

void ServerStreamer::sendStreamInstruction(const Common::ClientInfo *src, const Common::ClientInfo *dst, bool reset)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);

    out.setVersion(QDataStream::Qt_5_0);

    out << (quint16)0;
    block.append(Common::StreamCommand(dst->address.toString(), dst->port, reset).serialize());
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));

    src->connection->write(block);
    src->connection->waitForBytesWritten();

    qDebug() << "Server: Stream command sent:" << dst->address.toString() << dst->port << reset;
}

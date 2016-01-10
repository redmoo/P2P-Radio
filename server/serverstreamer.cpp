#include "serverstreamer.h"

#include <iostream>
#include <QTime>
#include <QtTest/QTest>


ServerStreamer::ServerStreamer(QObject *parent) : QObject(parent) {}

void ServerStreamer::init(QString ip, QString port)
{
    serverUdpSocket = new QUdpSocket(this); // TODO: mogoc most odpret se incoming, pa pol na clientu connecttohost da bo po netu delal!!!! virtualna dvosmerna povezava
    //connect(serverUdpSocket, &QIODevice::bytesWritten, this, &ServerStreamer::datagramSent); // for DEBUG

    tcpServer = new QTcpServer(this);

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

    //sendMessage(player->currentlyPlaying()); // TODO: new music... queue... remake player

    emit(connectionInfoChanged(ip, tcpServer->serverPort()));
}

void ServerStreamer::startStream(QString ip, QString port, bool chain)
{
    chain_streaming = chain;
    init(ip, port);
}

void ServerStreamer::setMusic(QString file)
{
    music_file = file; // TODO: check ce obstaja!!!!
}

void ServerStreamer::clientConnected() // TODO: close all connections when...?
{
    qint16 blockSize = 0;

    QTcpSocket *clientConnection = tcpServer->nextPendingConnection();
    qDebug() << "Local addr/port:" << clientConnection->localAddress() << clientConnection->localPort();
    qDebug() << "Peer addr/port:" << clientConnection->peerAddress() << clientConnection->peerPort() << clientConnection->peerName();

    // TODO: a ce tole gre cez se pol klice readyread pa gre v uno metodo takoj pred ostalimi povezavami? kr sele tm se pol connecta zares naj!
    if(clientConnection->waitForReadyRead()) // TODO: spremen zarad windowsov! see documentation. verjetn bi jih blo za v queue dat pa pol spodi processat...
    {                                        // TODO: tole queue. za test das kle 1, pa sleep v clienta in se ne connecta! potestiraj k bo connect queue!
        QDataStream in(clientConnection);    // TODO: NARED RECIEVE METODO ZA SERVER!
        in.setVersion(QDataStream::Qt_5_0);

        // TODO: pri testiranju mas kle umes un blockSize...

        /*if(blockSize == 0)
        {
            qDebug() << "block size = 0";
            if(clientConnection->bytesAvailable() < (int)sizeof(quint16)) return;
            in >> blockSize;
        }
        qDebug() << "Why am I here multiple times?";
        if(clientConnection->bytesAvailable() < blockSize) return;
        blockSize = 0;*/

        in >> blockSize; // TODO: read the above

        quint16 clientPort = 0;
        in >> clientPort;

        if (clientPort == 0) // nebo ok... kr pac bo druga cifra, se kr pa ne taprava
        {
            std::cerr << "Client data took too long to arrive!" << std::endl;
            return;
        }

        qDebug() << "Client sent its info. UDP port: " + QString::number(clientPort);

        //serverUdpSocket->connectToHost(clientConnection->localAddress(), clientPort);
        // We have obtained sub-socket, connection has been established

        auto *new_client = new Common::ClientInfo(clientConnection, clientConnection->peerAddress(), clientPort);

        if (chain_streaming && !clients.isEmpty())
            sendStreamInstruction(clients.last(), new_client);

        QVector<Common::ClientInfo *> rcps;
        rcps.append(new_client);
        sendMessage(player->currentlyPlaying(), rcps);

        clients << new_client;

        // TODO: tole vse samo ce se connecta!!
        connect(clientConnection, &QAbstractSocket::disconnected, clientConnection, &QAbstractSocket::deleteLater);
        connect(clientConnection, &QAbstractSocket::disconnected, this, &ServerStreamer::clientDisconnected); // TODO: a je treba to pol disconnectat?

        emit clientCountChanged(clients.length());
    }
}

void ServerStreamer::clientDisconnected() // TODO: kaj pa ce se dva naenkrat disconnectata???? a se da kako queue nardit?
{
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
            //qDebug() << "UDP to address:" << clients[i]->address;
            quint64 bytes_sent = serverUdpSocket->writeDatagram(chunk, chunk.size(), clients[i]->address, clients[i]->port);
            //qDebug() << "Bytes sent:" << bytes_sent;

            if(bytes_sent == -1)
                qDebug() << "CHUNK TOO BIG! (ServerStreamer::write)";
        }
    }
}

void ServerStreamer::datagramSent(){
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

// TODO: ^˘ zdruzi enake dele
void ServerStreamer::sendStreamInstruction(const Common::ClientInfo *src, const Common::ClientInfo *dst, bool reset)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);

    out.setVersion(QDataStream::Qt_5_0);

    out << (quint16)0;
    auto t = Common::StreamCommand(dst->address.toString(), dst->port, reset);
    auto test = t.serialize();
    auto sc = Common::StreamCommand();
    sc.deserialize(test, true);
    qDebug() << "C add:" << sc.address << sc.port << sc.reset_destinations;


    block.append(test);
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));

    src->connection->write(block);
    src->connection->waitForBytesWritten(); // TODO: windows! see doc also ZAKAJ JE KLE KDAJ UNCONNECTED????

    qDebug() << "Server: Stream command sent:" << t.address << t.port << t.reset_destinations;
}

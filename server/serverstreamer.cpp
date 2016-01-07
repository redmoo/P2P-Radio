#include "serverstreamer.h"
#include <QTime>

ServerStreamer::ServerStreamer(QObject *parent) : QObject(parent)
{
}

void ServerStreamer::init()
{
    socket = new QUdpSocket(this);

    //socket->bind(QHostAddress("193.2.178.92"), 1234);
    socket->bind(QHostAddress::LocalHost, 1234);

    tcpServer = new QTcpServer(this);

    if (! tcpServer-> listen (QHostAddress::LocalHost, 6666)){
        // Monitor port 6666 of the local host, if the error output an error message and close the
        qDebug () << tcpServer-> errorString ();
        return;
    }

    connect(tcpServer, &QTcpServer::newConnection, this, &ServerStreamer::clientConnected);
}

void ServerStreamer::startStream()
{
    player = new Player;
    //connect(player, &Player::bufferSend, this, &ServerStreamer::write);
    connect(player->source, &AudioSource::dataReady, this, &ServerStreamer::write);
    //connect(player, &Player::bufferSendChunks, this, &ServerStreamer::write);
}

void ServerStreamer::clientConnected() // TODO: close all connections when...?
{
    qint16 blockSize=0;
    QTcpSocket *clientConnection = tcpServer->nextPendingConnection();
    //clients << new Common::ClientInfo(clientConnection);
    qDebug() << "Client ID: " << clientConnection->socketDescriptor();

    if(clientConnection->waitForReadyRead()){
        qDebug() << "Cakam na sprejem" <<endl;
        QDataStream in(clientConnection);
        in.setVersion(QDataStream::Qt_5_0);

        if(blockSize == 0){
            if(clientConnection->bytesAvailable() < (int)sizeof(quint16)) return;
            in >> blockSize;
        }

        if(clientConnection->bytesAvailable() < blockSize) return;
        blockSize = 0;

        QString message;
        in >> message;
        qint16 clientPort = message.toInt();
        qDebug() << "Message (readMessage()): " + QString::number(clientPort);
        clients << new Common::ClientInfo(clientConnection, clientConnection->localAddress(), clientPort );
    }

    // We have obtained sub-socket, connection has been established

    connect(clientConnection, &QAbstractSocket::disconnected, clientConnection, &QAbstractSocket::deleteLater);
    connect(clientConnection, &QAbstractSocket::disconnected, this, &ServerStreamer::clientDisconnected); // TODO: a je treba to pol disconnectat?

    emit clientCountChanged(clients.length());
}

void ServerStreamer::clientDisconnected()
{
    QMutableVectorIterator<Common::ClientInfo *> iterator(clients);

    while(iterator.hasNext())
    {
        auto c = iterator.next();
        qDebug() << "DC:" << c->connection->socketDescriptor() << c->connection->isOpen() << c->connection->state();

        if (c->connection->state() == QAbstractSocket::UnconnectedState)
        {
            c->connection->close();
            iterator.remove();
        }
        // TODO: kle nardis prevezavo povezav med klienti in dolocs od kje kdo posilja, k en linked list? mogoce se kr to res ponuca...
    }

    emit clientCountChanged(clients.length());
}

void ServerStreamer::write(QVector<QByteArray> data)
{
    qDebug() << "Writing to clients!";

    foreach (auto chunk, data)
    {
        foreach(auto *c, clients){ // TODO: a ni to mal narobe...
            //qDebug() << c->address << "  " << c->port;

            if(-1 == socket->writeDatagram(chunk, QHostAddress::LocalHost, 1233))
                qDebug() << "CHUNK TOO BIG! (ServerStreamer::write)";
        }
    }
}

void ServerStreamer::sendMessage(QVector<Common::ClientInfo *> dsts) // TODO: args: paket, recipient vector, default = all
{
    QByteArray block; // for temporarily storing the data to be sent
    QDataStream out(&block, QIODevice::WriteOnly);

    // Use the data stream to write data

    out.setVersion (QDataStream::Qt_5_0);

    // Set the data stream version, the client and server side use the same version

    out << (quint16)0;
    block.append(Common::MessageCommand(QString("Ping! Time: %1").arg(QTime::currentTime().toString())).serialize());
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));

    foreach (Common::ClientInfo *c, dsts.isEmpty() ? clients : dsts)
        c->connection->write(block);

    qDebug() << "Server: Ping message sent.";
}

void ServerStreamer::addClient(Common::ClientInfo *c){
    clients << c;
}

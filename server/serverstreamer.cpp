#include "serverstreamer.h"
#include <QTime>


ServerStreamer::ServerStreamer(QObject *parent) : QObject(parent)
{
}

void ServerStreamer::init()
{
    socket = new QUdpSocket(this);
    //socket->bind(QHostAddress::LocalHost, 1234);
    socket->bind(QHostAddress("193.2.2176.111"), 1234);

    tcpServer = new QTcpServer(this);

    if (! tcpServer-> listen (QHostAddress("193.2.176.111"), 6666)){
        // Monitor port 6666 of the local host, if the error output an error message and close the
        qDebug () << tcpServer-> errorString ();
        return;
    }

    connect(tcpServer, &QTcpServer::newConnection, this, &ServerStreamer::clientConnected);

    // TODO: find a better place to do these stuffers below!!!!

    //player = new Player;
    //connect(player, &Player::bufferSend, this, &ServerStreamer::write);
}

void ServerStreamer::startStream()
{
    player = new Player;
    connect(player, &Player::bufferSend, this, &ServerStreamer::write);
    connect(player->source, &AudioSource::dataReady, this, &ServerStreamer::write);
}

void ServerStreamer::clientConnected() // TODO: close all connections when...?
{
    QTcpSocket *clientConnection = tcpServer->nextPendingConnection();
    clients << new Common::ClientInfo(clientConnection);
    qDebug() << "Client ID: " << clientConnection->socketDescriptor();

    // We have obtained sub-socket, connection has been established

    connect(clientConnection, &QAbstractSocket::disconnected, clientConnection, &QAbstractSocket::deleteLater);
    connect(clientConnection, &QAbstractSocket::disconnected, this, &ServerStreamer::clientDisconnected); // TODO: a je treba to pol disconnectat?

    emit clientCountChanged(clients.length());
}

void ServerStreamer::clientDisconnected()
{
    QMutableListIterator<Common::ClientInfo *> iterator(clients);

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

void ServerStreamer::write(QByteArray data)
{
    qDebug() << "Writing to clients!"; // TODO: tole SKOS strela?? a je ksn timer?
    qDebug() << clients.size();
    foreach(Common::ClientInfo *c, clients){
        qDebug() << c->address << "  " << c->port;
        //if(-1 == socket->writeDatagram(data, QHostAddress::LocalHost, 1233))
        if(-1 == socket->writeDatagram(data, QHostAddress("193.2.178.92"), 1233))
            qDebug() << "TO BIG!" <<endl;
    }
}

void ServerStreamer::sendMessage()
{
    QByteArray block; // for temporarily storing the data to be sent
    QDataStream out (& block, QIODevice :: WriteOnly);

    // Use the data stream to write data

    out.setVersion (QDataStream :: Qt_5_0);

    // Set the data stream version, the client and server side use the same version

    out << (quint16)0;
    out << QString("Ping! Time: %1").arg(QTime::currentTime().toString());
    out.device() -> seek(0);
    out << (quint16)(block.size() - sizeof(quint16));

    foreach (Common::ClientInfo *c, clients)
        c->connection->write(block);

    //clientConnection-> disconnectFromHost();

    qDebug() << "Server: Ping message sent.";
}

/*void ServerStreamer::sessionOpened()
{
    // Save the used configuration
    if (networkSession) {
        QNetworkConfiguration config = networkSession->configuration();
        QString id;
        if (config.type() == QNetworkConfiguration::UserChoice)
            id = networkSession->sessionProperty(QLatin1String("UserChoiceConfiguration")).toString();
        else
            id = config.identifier();

        QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        settings.setValue(QLatin1String("DefaultNetworkConfiguration"), id);
        settings.endGroup();
    }

    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen()) {
        qDebug() << "Unable to start the server: " + tcpServer->errorString();
        tcpServer->close();
        return;
    }
    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).toIPv4Address()) {
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }
    // if we did not find one, use IPv4 localhost
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    qDebug() << "Server is runing on " + ipAddress + " " + tcpServer->serverPort();

}*/

/*void ServerStreamer::sendFortune()
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << (quint16)0;
    out << fortunes.at(qrand() % fortunes.size());
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));

    QTcpSocket *clientConnection = tcpServer->nextPendingConnection();
    connect(clientConnection, SIGNAL(disconnected()),
            clientConnection, SLOT(deleteLater()));

    clientConnection->write(block);
    clientConnection->disconnectFromHost();
}*/

void ServerStreamer::addClient(Common::ClientInfo *c){
    clients << c;
}

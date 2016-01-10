#include "streamreceiver.h"
#include "common.h"
#include <QAudioOutput>
#include <QtTest/QTest>

StreamReceiver::StreamReceiver(QObject *parent) : QObject(parent)
{
}

void StreamReceiver::init(QString client_address, QString client_udp_port)
{
    /**** UDP SETUP ****/

    clientUdpSocket = new QUdpSocket(this);

    connect(clientUdpSocket, &QIODevice::readyRead, this, &StreamReceiver::dataReceived);
    qDebug() << "UDP bind successful:" << clientUdpSocket->bind(client_address.isEmpty() ? QHostAddress::Any : QHostAddress(client_address),
                                                                client_udp_port.isEmpty() ? 0 : client_udp_port.toUInt());

    if (client_address.isEmpty())
    {
        QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
        // use the first non-localhost IPv4 address
        for (int i = 0; i < ipAddressesList.size(); ++i)
        {
            if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
                ipAddressesList.at(i).toIPv4Address())
            {
                client_address = ipAddressesList.at(i).toString();
                break;
            }
        }
        // if we did not find one, use IPv4 localhost
        if (client_address.isEmpty())
            client_address = QHostAddress(QHostAddress::LocalHost).toString();
    }

    emit(connectionInfoChanged(client_address, clientUdpSocket->localPort()));

    /**** TCP SETUP ****/

    clientTcpSocket = new QTcpSocket(this);

    connect(clientTcpSocket, &QIODevice::readyRead, this, &StreamReceiver::readCommand);
    connect(clientTcpSocket, static_cast<void(QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error), this, &StreamReceiver::displayError);

    /**** AUDIO SETUP ****/

    auto *audio = new QAudioOutput(Common::getFormat(), this);
    audio->setBufferSize(1024*100);
    playbuff = audio->start();
}

void StreamReceiver::newConnect(QString server_ip, QString server_port, QString client_ip, QString client_port)
{
    init(client_ip, client_port);

    blockSize = 0;

    clientTcpSocket->abort();
    clientTcpSocket->connectToHost(QHostAddress(server_ip), server_port.toUInt());

    emit(activityLogChanged("Establishing connection to server at " + server_ip + " on port " + server_port));

    if(clientTcpSocket->waitForConnected()) // Randomly fails on windows!
    {
        // Data stream to write data
        QByteArray block; // for temporarily storing the data to be sent
        QDataStream out(&block, QIODevice::WriteOnly);

        // Set the data stream version, the client and server side use the same version
        out.setVersion(QDataStream::Qt_5_0);

        out << (quint16)0;
        out << clientUdpSocket->localPort();
        out.device()->seek(0);
        out << (quint16)(block.size() - sizeof(quint16));

        clientTcpSocket->write(block);

        clientTcpSocket->flush();
        clientTcpSocket->waitForBytesWritten(3000);

        emit(connectionStatusChanged("Connected to server"));
        emit(activityLogChanged("Connected to server at " + server_ip + " on port " + server_port));
    }
    else
    {
        emit(connectionStatusChanged("Unsuccessful connection on " + server_ip + ":" + server_port));
    }
}

void StreamReceiver::readCommand()
{
    QDataStream in(clientTcpSocket);
    in.setVersion(QDataStream::Qt_5_0);

    if(blockSize == 0){
        if(clientTcpSocket->bytesAvailable() < (int)sizeof(quint16)) return;
        in >> blockSize;
    }

    if(clientTcpSocket->bytesAvailable() < blockSize) return;
    blockSize = 0;

    quint8 cid;
    in >> cid;

    switch (cid)
    {
        case Common::CommandID::MESSAGE:
            readMessage(clientTcpSocket->readAll());
            break;

        case Common::CommandID::STREAM:
            updateDestinations(clientTcpSocket->readAll());
            break;

        default: qDebug() << "Command ID incorrect!";
    }
}

void StreamReceiver::readMessage(const QByteArray &data)
{
    QString message = (Common::MessageCommand().deserialize(data))->message;
    qDebug() << "Message (readMessage()): " + message;
    emit(messageChanged(message));
}

void StreamReceiver::updateDestinations(const QByteArray &data)
{
    auto stream_command = Common::StreamCommand();
    stream_command.deserialize(data);
    qDebug() << "Stream command:" << stream_command.address << stream_command.port << stream_command.reset_destinations;

    if (stream_command.reset_destinations)
    {
        clients.clear();
    }
    if (stream_command.address != QString("127.0.0.1") && stream_command.port != 0)
    {
        auto *new_client = new Common::ClientInfo(QHostAddress(stream_command.address), stream_command.port);
        clients.append(new_client);
    }
    else if (stream_command.address == QString(""))
        qDebug() << "Client chain: QHostAddress() -> this shouldn't happen!";
}

void StreamReceiver::dataReceived()
{
    QByteArray buffer;
    QHostAddress server;
    quint16 serverPort;

    while(clientUdpSocket->hasPendingDatagrams())
    {
        buffer.resize(clientUdpSocket->pendingDatagramSize());
        clientUdpSocket->readDatagram(buffer.data(), buffer.size(), &server, &serverPort);

        playbuff->write(buffer.data(), buffer.size());

        // Pass to clients
        foreach(auto *c, clients)
        {
            quint64 bytes_sent = clientUdpSocket->writeDatagram(buffer, c->address, c->port);

            if(bytes_sent == -1)
                qDebug() << "CHUNK TOO BIG! (Client pass down the chain.)";
        }
    }
}

void StreamReceiver::displayError(QAbstractSocket::SocketError socketError)
{
    QString connStatus = "";
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        connStatus = "Remote Host Closed";
        break;
    case QAbstractSocket::HostNotFoundError:
        connStatus = "The host was not found. Please check the host name and port settings.";
        break;
    case QAbstractSocket::ConnectionRefusedError:
        connStatus = "The connection was refused by the peer. Make sure the server is running,"
                     "and check that the host name and port settings are correct.";
        break;
    default:
        connStatus = "The following error occurred: " + clientTcpSocket->errorString();
    }

    qDebug() << connStatus;

    emit(connectionStatusChanged(connStatus));
    emit(activityLogChanged("Connection closed " + connStatus ));
    emit(connectButtonToggle(true));

}

void StreamReceiver::addClient(Common::ClientInfo *c){
    clients << c;
}

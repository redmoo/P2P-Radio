#include "streamreceiver.h"
#include "common.h"
#include <QAudioOutput>


StreamReceiver::StreamReceiver(QObject *parent) : QObject(parent)
{
}

void StreamReceiver::init()
{
    clientPort = 1233;
    clientAddress = QHostAddress::LocalHost;
    socket = new QUdpSocket(this);
    socket->bind(clientAddress, clientPort);

    connect(socket, &QIODevice::readyRead, this, &StreamReceiver::readyRead);

    tcpSocket = new QTcpSocket(this);

    serverAddress = QHostAddress::LocalHost;
    //serverAddress = QHostAddress("193.2.178.92");
    serverPort = 6666;
    connect(tcpSocket, &QIODevice::readyRead, this, &StreamReceiver::readCommand);

    connect(tcpSocket, static_cast<void(QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error), this, &StreamReceiver::displayError);

    //playbuff = new QBuffer();
    //playbuff->open(QBuffer::ReadWrite);

    auto *audio = new QAudioOutput(Common::getFormat(), this);
    //playbuff = audio->start();
    audio->setBufferSize(1024*100);
    playbuff = audio->start();
}

QString getIPAddress()
{
    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).toIPv4Address()) {
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    return ipAddress;
}

void StreamReceiver::newConnect()
{
    blockSize = 0;

    tcpSocket->abort();
    tcpSocket->connectToHost(serverAddress, serverPort);

    emit(activityLogChanged("Establishing connection to server at " + serverAddress.toString() + " on port " + QString::number(serverPort)));

    if(tcpSocket->waitForConnected(3000)){
        //Connection successful
        emit(connectionStatusChanged("Connected to server"));
        emit(activityLogChanged("Connected to server at " + serverAddress.toString() + " on port " + QString::number(serverPort)));

        QByteArray block; // for temporarily storing the data to be sent
        QDataStream out(&block, QIODevice::WriteOnly);

        // Use the data stream to write data

        out.setVersion(QDataStream::Qt_5_0);

        // Set the data stream version, the client and server side use the same version

        out << (quint16)0;
        //out << tr ("I'm a client" + clientPort );
        out << QString::number(clientPort);
        out.device()->seek(0);
        out << (quint16)(block.size() - sizeof(quint16));

        tcpSocket->write(block);

    }else{
        emit(connectionStatusChanged("Unsuccessful connection on " + serverAddress.toString() + ":" + QString::number(serverPort)));
    }

    //emit(activityLogChanged("Establishing connection to server at " + serverAddress.toString() + " on port " + serverPort));
}

void StreamReceiver::readCommand()
{
    QDataStream in(tcpSocket);
    in.setVersion(QDataStream::Qt_5_0);

    if(blockSize == 0){
        if(tcpSocket->bytesAvailable() < (int)sizeof(quint16)) return;
        in >> blockSize;
    }

    if(tcpSocket->bytesAvailable() < blockSize) return;
    blockSize = 0;

    quint8 cid = tcpSocket->peek(1).toUInt();

    switch (cid)
    {
        case Common::CommandID::MESSAGE:
            readMessage(tcpSocket->readAll());
            break;

        case Common::CommandID::DESTINATION:
            break;
    }
}

void StreamReceiver::readMessage(const QByteArray &data)
{
    QString message = (Common::MessageCommand().deserialize(data))->message;
    qDebug() << "Message (readMessage()): " + message;
    emit(messageChanged(message));
}

void StreamReceiver::readyRead()
{
    QByteArray buffer;
    buffer.resize(socket->pendingDatagramSize());

    QHostAddress server;
    quint16 serverPort;

    socket->readDatagram(buffer.data(), buffer.size(), &server, &serverPort);

    playbuff->write(buffer.data(), buffer.size());


    // Pass to clients
    //foreach(Common::ClientInfo *c, clients){
    //    socket->writeDatagram(buffer, c->address, c->port);
    //}

    qDebug() << "Message (readyRead()):" << buffer.size();
}

void StreamReceiver::displayError(QAbstractSocket::SocketError socketError)
{
    //qDebug() << tcpSocket->errorString (); // output an error message
    QString connStatus = "";
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        //connStatus = "RemoteHostClosedError";
        connStatus = "Remote Host Closed"; // TODO: pogrunti zakaj se to zgodi ce se odpre server pred clientom
        break;
    case QAbstractSocket::HostNotFoundError:
        connStatus = "The host was not found. Please check the host name and port settings.";
        break;
    case QAbstractSocket::ConnectionRefusedError:
        connStatus = "The connection was refused by the peer. Make sure the server is running,"
                     "and check that the host name and port settings are correct.";
        break;
    default:
        connStatus = "The following error occurred: " + tcpSocket->errorString();
    }
    qDebug() << connStatus;
    emit(connectionStatusChanged(connStatus));
    emit(activityLogChanged("Connection closed " + connStatus ));
    emit(connectButtonToggle(true));

}

void StreamReceiver::addClient(Common::ClientInfo *c){
    clients << c;
}

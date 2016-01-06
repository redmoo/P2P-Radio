#include "streamreceiver.h"
#include "common.h"
#include <QAudioOutput>


StreamReceiver::StreamReceiver(QObject *parent) : QObject(parent)
{
}

void StreamReceiver::init()
{
    socket = new QUdpSocket(this);
    //socket->bind(QHostAddress::LocalHost, 1233);
    socket->bind(QHostAddress("193.2.176.111"), 1233);

    connect(socket, &QIODevice::readyRead, this, &StreamReceiver::readyRead);

    tcpSocket = new QTcpSocket(this);

    //serverAddress = QHostAddress::LocalHost;
    serverAddress = QHostAddress("193.2.178.92");
    serverPort = 6666;
    connect(tcpSocket, &QIODevice::readyRead, this, &StreamReceiver::readMessage);

    connect(tcpSocket, static_cast<void(QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error), this, &StreamReceiver::displayError);

    //playbuff = new QBuffer();
    //playbuff->open(QBuffer::ReadWrite);

    auto *audio = new QAudioOutput(Common::getFormat(), this);
    audio->setBufferSize(1024*10);
        //playbuff = audio->start();
    playbuff = audio->start();


    /*// find out IP addresses of this machine
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();

    tcpSocket = new QTcpSocket(this);

    //connect(getFortuneButton, SIGNAL(clicked()),this, SLOT(requestNewFortune()));
    //connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readFortune()));
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(displayError(QAbstractSocket::SocketError)));


    QNetworkConfigurationManager manager;
    if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired) {
        // Get saved network configuration
        QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
        settings.endGroup();

        // If the saved network configuration is not currently discovered use the system default
        QNetworkConfiguration config = manager.configurationFromIdentifier(id);
        if ((config.state() & QNetworkConfiguration::Discovered) !=
            QNetworkConfiguration::Discovered) {
            config = manager.defaultConfiguration();
        }

        networkSession = new QNetworkSession(config, this);
        connect(networkSession, SIGNAL(opened()), this, SLOT(sessionOpened()));


        qDebug() << "Opening network session.";
        networkSession->open();
    }*/

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

    emit(activityLogChanged("Establishing connection to server at " + serverAddress.toString() + " on port " + serverPort));

    if(tcpSocket->waitForConnected(3000)){
        //Connection successful
        emit(connectionStatusChanged("Connected to server"));
        emit(activityLogChanged("Connected to server at " + serverAddress.toString() + " on port " + serverPort));

        QByteArray block;
        QDataStream out (& block, QIODevice :: WriteOnly);

        out.setVersion (QDataStream :: Qt_5_0);

        out << (quint16)0;
        out << tr ("I am client");
        out.device () -> seek (0);
        out << (quint16)(block.size () - sizeof(quint16));

        tcpSocket->write(block);

    }else{
        emit(connectionStatusChanged("Unsuccessful connection on " + serverAddress.toString() + ":" + serverPort));
        emit(activityLogChanged("Connected to server at " + serverAddress.toString() + " on port " + serverPort));
    }

    //emit(activityLogChanged("Establishing connection to server at " + serverAddress.toString() + " on port " + serverPort));
}

void StreamReceiver::readMessage()
{
    QDataStream in(tcpSocket);
    in.setVersion(QDataStream::Qt_5_0);

    if(blockSize == 0){
        if(tcpSocket->bytesAvailable() < (int)sizeof(quint16)) return;
        in >> blockSize;
    }

    if(tcpSocket->bytesAvailable() < blockSize) return;
    blockSize = 0;

    QString message;
    in >> message;

    qDebug() << "Message (readMessage()): " + message;

    emit(messageChanged(message));
}

void StreamReceiver::readyRead()
{
    qDebug() << "Read" << endl;
    QByteArray buffer;
    buffer.resize(socket->pendingDatagramSize());

    QHostAddress server;
    quint16 serverPort;

    socket->readDatagram(buffer.data(), buffer.size(), &server, &serverPort);

    qint64 val = playbuff->pos();
    playbuff->seek(playbuff->size());
    playbuff->write(buffer.data(), buffer.size());
    playbuff->seek(val);
    qDebug() << val << endl;


    // Pass to clients
    foreach(Common::ClientInfo *c, clients){
        socket->writeDatagram(buffer, c->address, c->port);
    }

    //qDebug() << "Message (readyRead()):" << buffer << endl;
}

/*
void StreamReceiver::doConnectTcp()
{
    tcpSocket = new QTcpSocket(this);

    //connect(tcpSocket, &QIODevice::readyRead, this, SLOT(readyReadTcpData())); ???

    tcpSocket->connectToHost(QHostAddress::LocalHost, 4444);

    if(socket->waitForConnected(5000)){
        qDebug() << "Connected to host!" << endl;
        tcpSocket->write("Hello server\r\n\r\n");
        tcpSocket->waitForBytesWritten(1000);
        tcpSocket->waitForReadyRead(3000);

        qDebug() << "Sent!";


        tcpSocket->close();
    }else{
        qDebug() << "Connection not established!" <<endl;
    }

}
*/

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

/*void StreamReceiver::sessionOpened()
{    // Save the used configuration

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

    qDebug() << "This examples requires that you run the Fortune Server example as well";


}*/

void StreamReceiver::addClient(Common::ClientInfo *c){
    clients << c;
}

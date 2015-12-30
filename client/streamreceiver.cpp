#include "streamreceiver.h"


StreamReceiver::StreamReceiver(QObject *parent) : QObject(parent)
{
    socket = new QUdpSocket(this);
    socket->bind(QHostAddress::LocalHost, 1233);

    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));

    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readFortune()));
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
                this, SLOT(displayError(QAbstractSocket::SocketError)));


    // find out IP addresses of this machine
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
    }


}

void StreamReceiver::readyRead(){
    qDebug() << "Read" << endl;
    QByteArray buffer;
    buffer.resize(socket->pendingDatagramSize());

    QHostAddress server;
    quint16 serverPort;

    socket->readDatagram(buffer.data(), buffer.size(), &server, &serverPort);

    qDebug() << "Message:" << buffer << endl;
}

void StreamReceiver::doConnectTcp()
{
    tcpSocket = new QTcpSocket(this);

    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readyReadTcpData()));

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

void StreamReceiver::requestNewFortune(){
    blockSize = 0;
    tcpSocket->abort();
    tcpSocket->connectToHost(QHostAddress::LocalHost, 4444);
}

void StreamReceiver::readFortune(){
    QDataStream in(tcpSocket);
    in.setVersion(QDataStream::Qt_5_0);

    if (blockSize == 0) {
       if (tcpSocket->bytesAvailable() < (int)sizeof(quint16))
           return;

       in >> blockSize;
    }

    if (tcpSocket->bytesAvailable() < blockSize)
       return;

    QString nextFortune;
    in >> nextFortune;

    if (nextFortune == currentMessage) {
       QTimer::singleShot(0, this, SLOT(requestNewFortune()));
       return;
    }

    currentMessage = nextFortune;
    qDebug() << "Message: " << currentMessage;

}

void StreamReceiver::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        qDebug() << "RemoteHostClosedError";
        break;
    case QAbstractSocket::HostNotFoundError:
        qDebug() << "The host was not found. Please check the host name and port settings.";
        break;
    case QAbstractSocket::ConnectionRefusedError:
        qDebug() << "The connection was refused by the peer. Make sure the fortune server is running,"
                    "and check that the host name and port settings are correct.";
        break;
    default:
        qDebug() << "The following error occurred: %1." + tcpSocket->errorString();
    }

}

void StreamReceiver::sessionOpened()
{
    // Save the used configuration
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


}

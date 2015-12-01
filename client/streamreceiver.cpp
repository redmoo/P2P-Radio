#include "streamreceiver.h"

StreamReceiver::StreamReceiver(QObject *parent) : QObject(parent)
{
    socket = new QUdpSocket(this);
    socket->bind(QHostAddress::LocalHost, 1233);

    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
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


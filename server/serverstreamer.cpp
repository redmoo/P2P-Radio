#include "serverstreamer.h"

ServerStreamer::ServerStreamer(QObject *parent) : QObject(parent)
{
    socket = new QUdpSocket(this);
    socket->bind(QHostAddress::LocalHost, 1234);
}

void ServerStreamer::write(QByteArray data)
{
    socket->writeDatagram(data, QHostAddress::LocalHost, 1233);
}


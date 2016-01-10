#ifndef STREAMRECEIVER_H
#define STREAMRECEIVER_H

#include "common.h"

#include <QObject>
#include <QUdpSocket>
#include <QTcpSocket>
#include <QNetworkSession>
#include <QtNetwork>
#include <QNetworkInterface>


class StreamReceiver : public QObject
{
    Q_OBJECT

public:
    explicit StreamReceiver(QObject *parent = 0);
    void init(QString client_address, QString client_udp_port);
    void newConnect(QString server_ip, QString server_port, QString client_ip, QString client_port);
    void addClient(Common::ClientInfo *);

private:
    void readMessage(const QByteArray &data);
    void updateDestinations(const QByteArray &data);

signals:
    void connectionInfoChanged(QString, quint16);
    void messageChanged(QString);
    void connectionStatusChanged(QString);
    void activityLogChanged(QString);
    void connectButtonToggle(bool);

private slots:
    void dataReceived();
    void displayError(QAbstractSocket::SocketError socketError);
    void readCommand();

private:
    //QHostAddress serverAddress;
    //quint16 serverUdpPort;
    //quint16 serverTcpPort;

    //QHostAddress clientAddress;
    //quint16 clientUdpPort;

    QUdpSocket *clientUdpSocket;
    QTcpSocket *clientTcpSocket;

    quint16 blockSize;
    QIODevice *playbuff;

    QVector<Common::ClientInfo *> clients;
};

#endif // STREAMRECEIVER_H

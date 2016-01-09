#ifndef STREAMRECEIVER_H
#define STREAMRECEIVER_H

#include <QObject>
#include <QUdpSocket>
#include <QTcpSocket>
#include <QNetworkSession>
#include <QtNetwork>
#include <QNetworkInterface>
#include "common.h"


class StreamReceiver : public QObject
{
    Q_OBJECT
public:
    explicit StreamReceiver(QObject *parent = 0);
    void init();
    void newConnect(QString server_ip, QString client_ip);
    void addClient(Common::ClientInfo *);

private:
    void readMessage(const QByteArray &data);
    void updateDestinations(const QByteArray &data);

signals:
    void messageChanged(QString);
    void connectionStatusChanged(QString);
    void activityLogChanged(QString);
    void connectButtonToggle(bool);

private slots:
    void dataReceived();
    void displayError(QAbstractSocket::SocketError socketError);
    void readCommand();
    //QString readMessage();
    //void doConnectTcp();
    //void requestNewFortune();
    //void readFortune();
    //void sessionOpened();

private:
    QHostAddress serverAddress;
    //quint16 serverUdpPort;
    quint16 serverTcpPort;

    QHostAddress clientAddress;
    quint16 clientUdpPort;

    QUdpSocket *clientUdpSocket;
    QTcpSocket *clientTcpSocket;

    quint16 blockSize;
    QIODevice *playbuff;

    QVector<Common::ClientInfo *> clients;
};

#endif // STREAMRECEIVER_H

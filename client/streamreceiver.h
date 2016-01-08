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

signals:
    void messageChanged(QString);
    void connectionStatusChanged(QString);
    void activityLogChanged(QString);
    void connectButtonToggle(bool);

private slots:
    void readyRead();
    void displayError(QAbstractSocket::SocketError socketError);
    void readCommand();
    //QString readMessage();
    //void doConnectTcp();
    //void requestNewFortune();
    //void readFortune();
    //void sessionOpened();

private:
    QUdpSocket *clientUdpSocket;
    QTcpSocket *clientTcpSocket;
    QNetworkSession *networkSession;
    quint16 blockSize;
    QIODevice *playbuff;
    //QBuffer *playbuff;
    QList<Common::ClientInfo *> clients;
    QHostAddress serverAddress;
    qint16 serverUdpPort;
    qint16 serverTcpPort;
    qint16 clientUdpPort;
    QHostAddress clientAddress;


};

#endif // STREAMRECEIVER_H

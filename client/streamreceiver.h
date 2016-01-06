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
    void newConnect();
    void addClient(Common::ClientInfo *);

signals:
    void messageChanged(QString);
    void connectionStatusChanged(QString);
    void activityLogChanged(QString);
    void connectButtonToggle(bool);

private slots:
    void readyRead();
    void displayError(QAbstractSocket::SocketError socketError);
    void readMessage();
    //QString readMessage();
    //void doConnectTcp();
    //void requestNewFortune();
    //void readFortune();
    //void sessionOpened();

private:
    QUdpSocket *socket;
    QTcpSocket *tcpSocket;
    QNetworkSession *networkSession;
    quint16 blockSize;
    QBuffer *playbuff;
    QList<Common::ClientInfo *> clients;
    QHostAddress serverAddress;
    qint16 serverPort;


};

#endif // STREAMRECEIVER_H

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

private slots:
    void readyRead();
    void displayError(QAbstractSocket::SocketError socketError);
    void readMessage();
    //void doConnectTcp();
    //void requestNewFortune();
    //void readFortune();
    //void sessionOpened();

private:
    QUdpSocket *socket;
    QTcpSocket *tcpSocket;
    QString message;
    QNetworkSession *networkSession;
    quint16 blockSize;
    QIODevice *playbuff;


};

#endif // STREAMRECEIVER_H

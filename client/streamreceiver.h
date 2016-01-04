#ifndef STREAMRECEIVER_H
#define STREAMRECEIVER_H

#include <QObject>
#include <QUdpSocket>
#include <QTcpSocket>
#include <QNetworkSession>
#include <QtNetwork>
#include <QNetworkInterface>
#include "common.h"
#include "clientinfo.h"



class StreamReceiver : public QObject
{
    Q_OBJECT
public:
    explicit StreamReceiver(QObject *parent = 0);
    void init();
    void newConnect();
    void addClient(ClientInfo);
    QString message;

signals:
    void messageChanged(QString);
    void connectionStatusChanged(QString);
    void activityLogChanged(QString);

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
    //QString message;
    QNetworkSession *networkSession;
    quint16 blockSize;
    QIODevice *playbuff;
    QList<ClientInfo> clients;
    QHostAddress serverAddress;
    qint16 serverPort;


};

#endif // STREAMRECEIVER_H

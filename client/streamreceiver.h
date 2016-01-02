#ifndef STREAMRECEIVER_H
#define STREAMRECEIVER_H

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
    void init();
    void newConnect();
    QString message;

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

};

#endif // STREAMRECEIVER_H

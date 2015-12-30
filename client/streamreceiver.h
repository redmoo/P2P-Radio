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
private:
    QUdpSocket *socket;
    QTcpSocket *tcpSocket;
    QString message;
    QNetworkSession *networkSession;
    quint16 blockSize;

signals:

public slots:
    void readyRead();
    void doConnectTcp();
    //void requestNewFortune();
    //void readFortune();
    void displayError(QAbstractSocket::SocketError socketError);
    //void sessionOpened();
    void newConnect();
    void readMessage();


};

#endif // STREAMRECEIVER_H

#ifndef SERVERSTREAMER_H
#define SERVERSTREAMER_H

#include "common.h"
#include "player.h"

#include <QObject>
#include <QUdpSocket>
#include <QTcpServer>
#include <QNetworkSession>
#include <QTcpSocket>
#include <QNetworkConfigurationManager>
#include <QSettings>
#include <QDataStream>
#include <QAudioBuffer>


class ServerStreamer : public QObject
{
    Q_OBJECT

public:
    explicit ServerStreamer(QObject *parent = 0);
    void init();
    void sendMessage();
    void startStream();


private:

    void addClient(Common::ClientInfo *); // TODO: namespace!!

signals:
    void clientCountChanged(int);


private slots:
    void clientConnected();
    void clientDisconnected();
    //void sessionOpened();
    //void sendFortune();
    void write(QByteArray data);

private:
    QUdpSocket *socket;
    QTcpServer *tcpServer;
    QStringList messages;
    QNetworkSession *networkSession;
    QList<Common::ClientInfo *> clients;
    Player *player;
};

#endif // SERVERSTREAMER_H

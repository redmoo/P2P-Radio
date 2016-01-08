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
    void sendMessage(QVector<Common::ClientInfo *> dsts = QVector<Common::ClientInfo *>()); // TODO: prek signala?
    void startStream(QString ip, bool chain_streaming);

private:
    void addClient(Common::ClientInfo *); // TODO: namespace!!

signals:
    void clientCountChanged(int);

private slots:
    void clientConnected();
    void clientDisconnected();
    //void sessionOpened();
    //void sendFortune();
    void write(QVector<QByteArray> data);

private:
    QHostAddress serverAddress;
    qint16 serverUdpPort;
    qint16 serverTcpPort;
    QUdpSocket *serverUdpSocket;
    QTcpServer *tcpServer;
    QStringList messages;
    QNetworkSession *networkSession;
    QVector<Common::ClientInfo *> clients;
    Player *player;
};

#endif // SERVERSTREAMER_H

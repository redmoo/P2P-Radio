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
    void sendMessage(const QVector<Common::ClientInfo *> dsts = QVector<Common::ClientInfo *>()); // TODO: prek signala?
    void startStream(QString ip, bool chain);

private:
    void sendStreamInstruction(const Common::ClientInfo *src, const Common::ClientInfo *dst, bool reset = true);

signals:
    void clientCountChanged(int);

private slots:
    void clientConnected();
    void clientDisconnected();
    //void sessionOpened();
    //void sendFortune();
    void write(const QVector<QByteArray> data);
    void datagramSent();

private:
    //QHostAddress serverAddress;
    quint16 serverUdpPort;
    quint16 serverTcpPort;

    QUdpSocket *serverUdpSocket;
    QTcpServer *tcpServer;

    bool chain_streaming;
    QVector<Common::ClientInfo *> clients;

    Player *player;
};

#endif // SERVERSTREAMER_H

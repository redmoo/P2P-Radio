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
    void init(QString ip, QString port);
    void sendMessage(const QString msg = QString(), const QVector<Common::ClientInfo *> dsts = QVector<Common::ClientInfo *>());
    void startStream(QString ip, QString port, bool chain);
    void setMusic(QString file);

private:
    void sendStreamInstruction(const Common::ClientInfo *src, const Common::ClientInfo *dst, bool reset = true);

signals:
    void connectionInfoChanged(QString, quint16);
    void clientCountChanged(int);

private slots:
    void clientConnected();
    void clientDisconnected();
    void write(const QVector<QByteArray> data);
    void datagramSent();

private:
    QUdpSocket *serverUdpSocket;
    QTcpServer *tcpServer;

    bool chain_streaming;
    QVector<Common::ClientInfo *> clients;

    Player *player;
    QString music_file;
};

#endif // SERVERSTREAMER_H

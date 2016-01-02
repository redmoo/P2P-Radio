#ifndef SERVERSTREAMER_H
#define SERVERSTREAMER_H

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
private:
    QUdpSocket *socket;
    QTcpServer *tcpServer;
    QStringList messages;
    QNetworkSession *networkSession;


signals:

public slots:
    //void sessionOpened();
    //void sendFortune();
    void write(QByteArray data);
    void sendMessage();

};

#endif // SERVERSTREAMER_H

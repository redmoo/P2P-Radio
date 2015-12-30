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

class ServerStreamer : public QObject
{
    Q_OBJECT
public:
    explicit ServerStreamer(QObject *parent = 0);
    void write(QByteArray data);
private:
    QUdpSocket *socket;
    QTcpServer *tcpServer;
    QStringList fortunes;
    QNetworkSession *networkSession;


signals:

public slots:
    void sessionOpened();
    void sendFortune();

};

#endif // SERVERSTREAMER_H

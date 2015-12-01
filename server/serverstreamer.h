#ifndef SERVERSTREAMER_H
#define SERVERSTREAMER_H

#include <QObject>
#include <QUdpSocket>

class ServerStreamer : public QObject
{
    Q_OBJECT
public:
    explicit ServerStreamer(QObject *parent = 0);
    void write(QByteArray data);
private:
    QUdpSocket *socket;

signals:

public slots:
};

#endif // SERVERSTREAMER_H

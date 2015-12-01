#ifndef STREAMRECEIVER_H
#define STREAMRECEIVER_H

#include <QObject>
#include <QUdpSocket>

class StreamReceiver : public QObject
{
    Q_OBJECT
public:
    explicit StreamReceiver(QObject *parent = 0);
private:
    QUdpSocket *socket;

signals:

public slots:
    void readyRead();

};

#endif // STREAMRECEIVER_H

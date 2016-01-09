#ifndef PLAYER_H
#define PLAYER_H

#include "audiosource.h"

#include <QObject>
#include <QMediaPlayer>
#include <QAudioBuffer>
#include <QAudioProbe>
#include <QFile>
#include <QAudioDecoder>
#include <QAudioOutput>


class Player : public QObject
{
    Q_OBJECT
public:
    explicit Player(QObject *parent = 0);
    AudioSource *source;

signals:
    void bufferSend(QByteArray);
    void bufferSendChunks(QVector<QByteArray>);

public slots:
    void processBuffer(QAudioBuffer);

private:
    QMediaPlayer *player;
    QAudioDecoder *decoder;

};

#endif // PLAYER_H

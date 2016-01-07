#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QMediaPlayer>
#include <QAudioBuffer>
#include <QAudioProbe>
#include <QFile>
#include <QAudioDecoder>
#include <QAudioOutput>
#include "audiosource.h"

class Player : public QObject
{
    Q_OBJECT
public:
    explicit Player(QString audioFile, QObject *parent = 0);
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

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
    explicit Player(QString file, QObject *parent = 0);
    QString currentlyPlaying();
    AudioSource *source; // NESME BIT TO PUBLIC WTFFFF

signals:
    void bufferSend(QByteArray);
    void bufferSendChunks(QVector<QByteArray>);

public slots:
    void processBuffer(QAudioBuffer);

private:
    QMediaPlayer *player;
    QAudioDecoder *decoder;
    QString music_file;

};

#endif // PLAYER_H

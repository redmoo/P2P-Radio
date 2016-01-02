#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QMediaPlayer>
#include <QAudioBuffer>
#include <QAudioProbe>
#include <QFile>

class Player : public QObject
{
    Q_OBJECT
public:
    explicit Player(QObject *parent = 0);

signals:

public slots:
    void processBuffer(QAudioBuffer);

private:
    QMediaPlayer *player;

};

#endif // PLAYER_H

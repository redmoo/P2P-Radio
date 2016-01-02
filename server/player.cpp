#include "player.h"

Player::Player(QObject *parent) : QObject(parent)
{
    player = new QMediaPlayer;
    // ...
    //QFile file(":/habibi.mp3");
    //player->setMedia(QUrl::fromLocalFile("/home/nikolai5/Dropbox/School/RZP/Projekt/P2P-Radio/habibi.mp3"));
    //player->setMedia(QUrl::fromLocalFile("/habibi.mp3"));
    //Settings::resourcePath();
    player->setMedia(QUrl("qrc:/habibi.mp3"));
    player->setVolume(0);

    QAudioProbe *probe = new QAudioProbe;

    // ... configure the audio recorder (skipped)

    connect(probe, &QAudioProbe::audioBufferProbed, this, &Player::processBuffer);

    probe->setSource(player); // Returns true, hopefully.


    player->play();
}

qint64 microsec=-1;
void Player::processBuffer(QAudioBuffer abuff)
{
    emit bufferSend(QByteArray((const char*)abuff.constData(), abuff.byteCount()));
}

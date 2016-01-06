#include "player.h"
#include "audiosource.h"
#include "common.h"

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


    //player->play();

    source = new AudioSource();
    source->decode();

    auto *audio = new QAudioOutput(Common::getFormat(), this);
    audio->setVolume(0.5);
    audio->start(source);
}


void Player::processBuffer(QAudioBuffer abuff)
{
    QByteArray b1 = QByteArray((const char*) abuff.constData(), abuff.byteCount()/2);
    QByteArray b2 = QByteArray((const char*) abuff.constData()+abuff.byteCount()/2, abuff.byteCount()/2);
    //emit bufferSend(QByteArray((const char*)abuff.constData(), abuff.byteCount()));
    emit bufferSend(b1);
    emit bufferSend(b2);
}

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
    player->setVolume(50);

    /*
    QAudioProbe *probe = new QAudioProbe;

    // ... configure the audio recorder (skipped)

    connect(probe, SIGNAL(audioBufferProbed(QAudioBuffer)), this, SLOT(processBuffer(QAudioBuffer)));

    probe->setSource(player); // Returns true, hopefully.
    */


    player->play();
}

qint64 microsec=-1;
void Player::processBuffer(QAudioBuffer abuff)
{
    if(abuff.startTime() > microsec){
        microsec = abuff.startTime() + abuff.duration();
    }
}

#include "player.h"

Player::Player(QObject *parent) : QObject(parent)
{
    player = new QMediaPlayer;
    // ...
    //QFile file(":/habibi.mp3");
    //player->setMedia(QUrl::fromLocalFile("/home/nikolai5/Dropbox/School/RZP/Projekt/P2P-Radio/habibi.mp3"));
    //player->setMedia(QUrl::fromLocalFile("/habibi.mp3"));
    //Settings::resourcePath();
    player->setMedia(QUrl("qrc:/proto.mp3"));
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
    qDebug() << " -------- Probing asshole... -------- ";

    QByteArray b1 = QByteArray((const char*) abuff.constData(), abuff.byteCount()/2);
    QByteArray b2 = QByteArray((const char*) abuff.constData() + abuff.byteCount()/2, abuff.byteCount()/2);

    QVector<QByteArray> chunks;
    //chunks.append(b1);
    //chunks.append(b2);

    int chunk_size = abuff.byteCount() / 10;

    for (int i = 0; i < 10; i++)
    {
        chunks.append(QByteArray((const char*) abuff.constData() + i * chunk_size, chunk_size));
    }

    //emit bufferSend(QByteArray((const char*) abuff.constData(), abuff.byteCount()));
    emit bufferSendChunks(chunks);
}

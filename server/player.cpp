#include "common.h"
#include "player.h"
#include "audiosource.h"


Player::Player(QString file, QObject *parent) : music_file(file), QObject(parent)
{
    player = new QMediaPlayer;
    // ...
    //QFile file(":/habibi.mp3");
    //player->setMedia(QUrl::fromLocalFile("/home/nikolai5/Dropbox/School/RZP/Projekt/P2P-Radio/habibi.mp3"));
    //player->setMedia(QUrl::fromLocalFile("/habibi.mp3"));
    //Settings::resourcePath();
    //player->setMedia(QUrl("qrc:/habibi.mp3"));
    //player->setVolume(0);

    //QAudioProbe *probe = new QAudioProbe;

    // ... configure the audio recorder (skipped)

    //connect(probe, &QAudioProbe::audioBufferProbed, this, &Player::processBuffer);

    //probe->setSource(player); // Returns true, hopefully.


    //player->play();

    source = new AudioSource(); // TODO: A SPLOH SE RABMO PLAYER FILE??????????????????????
    source->decode(file);

    auto *audio = new QAudioOutput(Common::getFormat(), this);
    audio->setVolume(0.0);
    audio->start(source);
}

QString Player::currentlyPlaying()
{
    return QFileInfo(music_file).fileName();
}

void Player::processBuffer(QAudioBuffer abuff)
{
    qDebug() << " -------- Probing asshole... -------- ";

    //QByteArray b1 = QByteArray((const char*) abuff.constData(), abuff.byteCount()/2);
    //QByteArray b2 = QByteArray((const char*) abuff.constData() + abuff.byteCount()/2, abuff.byteCount()/2);

    //QByteArray b1 = QByteArray((const char*) abuff.constData(), abuff.byteCount()/4);
    //QByteArray b2 = QByteArray((const char*) abuff.constData()+abuff.byteCount()/4, abuff.byteCount()/4);
    //QByteArray b3 = QByteArray((const char*) abuff.constData()+abuff.byteCount()/2, abuff.byteCount()/4);
    //QByteArray b4 = QByteArray((const char*) abuff.constData()+3*abuff.byteCount()/4, abuff.byteCount()/4);

    QVector<QByteArray> chunks;
    //chunks.append(b1);
    //chunks.append(b2);

    int chunk_size = abuff.byteCount() / 10;

    for (int i = 0; i < 10; i++)
    {
        chunks.append(QByteArray((const char*) abuff.constData() + i * chunk_size, chunk_size));
    }

    emit bufferSendChunks(chunks);
}

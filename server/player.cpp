#include "common.h"
#include "player.h"
#include "audiosource.h"


Player::Player(QString file, QObject *parent) : music_file(file), QObject(parent)
{
    player = new QMediaPlayer;

    source = new AudioSource();
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
    qDebug() << " -------- Probing... -------- ";

    QVector<QByteArray> chunks;

    int chunk_size = abuff.byteCount() / 10;

    for (int i = 0; i < 10; i++)
    {
        chunks.append(QByteArray((const char*) abuff.constData() + i * chunk_size, chunk_size));
    }

    emit bufferSendChunks(chunks);
}

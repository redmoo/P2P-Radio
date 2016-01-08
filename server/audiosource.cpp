#include "audiosource.h"
#include "common.h"

AudioSource::AudioSource()
{
    this->open(QBuffer::ReadWrite);
    }

void AudioSource::decode(){
    decoder = new QAudioDecoder(this);
    decoder->setAudioFormat(Common::getFormat());
    //decoder->setSourceFilename(QUrl("qrc:/habibi.mp3").toLocalFile());
    qDebug() << QFile("C:\\Projects\\P2P-Radio\\proto.mp3").exists();
    decoder->setSourceFilename("C:\\Projects\\P2P-Radio\\proto.mp3");
    decoder->connect(decoder, &QAudioDecoder::bufferReady, this, &AudioSource::processBufferDecoder);
    decoder->start();
    // this->setBuffer(new QByteArray(1024*200,0));
}

void AudioSource::processBufferDecoder(){
    QAudioBuffer buff = decoder->read();
    qint64 val = this->pos();
    this->seek(this->size());
    this->write((const char*)buff.constData(),buff.byteCount());
    this->seek(val);
}

qint64 AudioSource::readData(char *data, qint64 maxlen){
    QVector<QByteArray> chunks;
    qint64 tmp = QBuffer::readData(data, maxlen);
    chunks.append(QByteArray((const char*)data, tmp));
    emit dataReady(chunks);
    return tmp;
}

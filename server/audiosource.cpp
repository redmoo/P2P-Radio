#include "audiosource.h"
#include "common.h"

AudioSource::AudioSource()
{
    this->open(QBuffer::ReadWrite);
    }

void AudioSource::decode(QString audioFile){
    decoder = new QAudioDecoder(this);
    decoder->setAudioFormat(Common::getFormat());
    //decoder->setSourceFilename(QUrl("qrc:/habibi.mp3").toLocalFile());
    //decoder->setSourceFilename("F:\\habibi.mp3");
    decoder->setSourceFilename(audioFile);


    connect(decoder, &QAudioDecoder::bufferReady, this, &AudioSource::processBufferDecoder);
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

qint64 tmp;
qint64 AudioSource::readData(char *data, qint64 maxlen){
    QVector<QByteArray> chunks;
    tmp = QBuffer::readData(data, maxlen);
    chunks.append(QByteArray((const char*)data, tmp));
    emit dataReady(chunks);
    return tmp;
}

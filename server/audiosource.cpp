#include "audiosource.h"
#include "common.h"


AudioSource::AudioSource()
{
    this->open(QBuffer::ReadWrite);
}

void AudioSource::decode(QString file)
{
    decoder = new QAudioDecoder(this);
    decoder->setAudioFormat(Common::getFormat());
    qDebug() << "Source file found:" << QFile(file).exists(); // return if not?
    decoder->setSourceFilename(file);
    decoder->connect(decoder, &QAudioDecoder::bufferReady, this, &AudioSource::processBufferDecoder);
    decoder->start();
    // this->setBuffer(new QByteArray(1024*200,0));
}

void AudioSource::processBufferDecoder()
{
    QAudioBuffer buff = decoder->read();
    qint64 val = this->pos();
    this->seek(this->size());
    this->write((const char*)buff.constData(),buff.byteCount());
    this->seek(val);
}

qint64 AudioSource::readData(char *data, qint64 maxlen)
{
    QVector<QByteArray> chunks;
    qint64 tmp = QBuffer::readData(data, maxlen);
    chunks.append(QByteArray((const char*)data, tmp));
    emit dataReady(chunks);
    return tmp;
}

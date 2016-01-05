#include "audiosource.h"
#include "common.h"

AudioSource::AudioSource()
{
    decoder = new QAudioDecoder(this);
    decoder->setAudioFormat(Common::getFormat());
    decoder->setSourceFilename("/home/nikolai5/Dropbox/School/RZP/Projekt/P2P-Radio/habibi.mp3");

    connect(decoder, &QAudioDecoder::bufferReady, this, &AudioSource::processBufferDecoder);
    decoder->start();
    this->setBuffer(new QByteArray(1024*200,0));
}

void AudioSource::processBufferDecoder(){
    QAudioBuffer buff = decoder->read();
    qint64 val = this->pos();
    this->seek(this->size());
    this->write((const char*)buff.constData(),buff.byteCount());
    this->seek(val);
    qDebug() << "Writing" << endl;
}

qint64 tmp;
qint64 AudioSource::readData(char *data, qint64 maxlen){
    tmp = QBuffer::readData(data, maxlen);
    emit dataReady(QByteArray((const char*)data, tmp));
    return tmp;
}

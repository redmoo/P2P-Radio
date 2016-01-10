#ifndef AUDIOSOURCE_H
#define AUDIOSOURCE_H

#include <QAudioDecoder>
#include <QBuffer>


class AudioSource : public QBuffer
{
    Q_OBJECT

public:
    AudioSource();
    void decode(QString file);
    qint64 readData(char *data, qint64 maxlen);

private:
    QAudioDecoder *decoder;

public slots:
    void processBufferDecoder();

signals:
    void dataReady(QVector<QByteArray>);

};

#endif // AUDIOSOURCE_H

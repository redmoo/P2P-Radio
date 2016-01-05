#include "common.h"

QAudioFormat Common::getFormat()
{
    /*static QAudioFormat format;
    // Set up the format, eg.
    format.setSampleRate(44100);
    format.setChannelCount(2);
    format.setSampleSize(32);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);

    return format;
    */
    static QAudioFormat desiredFormat;
    desiredFormat.setChannelCount(2);
    desiredFormat.setCodec("audio/x-raw");
    desiredFormat.setSampleType(QAudioFormat::UnSignedInt);
    desiredFormat.setByteOrder(QAudioFormat::LittleEndian);
    desiredFormat.setSampleRate(44100);
    desiredFormat.setSampleSize(16);

    return desiredFormat;

}

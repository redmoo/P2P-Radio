#ifndef COMMON_H
#define COMMON_H

#include "common_global.h"
#include <QAudioFormat>

class COMMONSHARED_EXPORT Common
{

public:
    //Common();
    QAudioFormat getFormat();
};

#endif // COMMON_H

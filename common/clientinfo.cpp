#include "clientinfo.h"

ClientInfo::ClientInfo(QHostAddress addr, quint16 prt)
{
    address = addr;
    port = prt;
}


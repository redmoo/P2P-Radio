#include "main_window.h"
#include <QApplication>
#include "serverstreamer.h"
#include "player.h"
#include "clientinfo.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ServerStreamer *stream = new ServerStreamer;

    ClientInfo client(QHostAddress::LocalHost, 1233);
    stream->addClient(client);

    MainWindow w(stream);
    //stream.sendFortune();

    Player p;
    QObject::connect(&p, SIGNAL(bufferSend(QByteArray)), stream, SLOT(write(QByteArray)));

    w.show();

    //stream.sendFortune(); ??

    return a.exec();
}

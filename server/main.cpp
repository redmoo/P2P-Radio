#include "main_window.h"
#include <QApplication>
#include "serverstreamer.h"
#include "player.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    //w.show();


    ServerStreamer stream;
    //stream.sendFortune();

    //Player p;


    return a.exec();
}

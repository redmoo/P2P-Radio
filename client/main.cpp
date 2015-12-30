#include "main_window.h"
#include <QApplication>
#include "streamreceiver.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //MainWindow w;
    //w.show();

    StreamReceiver recv;
    //recv.doConnect();
    //recv.requestNewFortune();

    recv.newConnect();


    return a.exec();
}

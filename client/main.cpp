#include "main_window.h"
#include <QApplication>
#include "streamreceiver.h"

int main(int argc, char *argv[])
{
    StreamReceiver recv;

    QApplication a(argc, argv);
    //MainWindow w;
    //w.show();

    return a.exec();
}

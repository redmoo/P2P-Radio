#include "main_window.h"
#include <QApplication>
#include "streamreceiver.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    StreamReceiver *recv = new StreamReceiver;
    MainWindow w(recv);

    w.show();

    return a.exec();
}

#include "main_window.h"
#include <QApplication>
#include "serverstreamer.h"

int main(int argc, char *argv[])
{
    ServerStreamer stream;

    QByteArray data;
    data.append("Hello world!!");
    stream.write(data);

    qDebug() << "Writed." << endl;

    QApplication a(argc, argv);
    //MainWindow w;
    //w.show();

    return a.exec();
}

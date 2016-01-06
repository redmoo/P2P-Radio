#include "main_window.h"
#include <QApplication>
#include "serverstreamer.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qDebug() << QUrl("qrc:/habibi.mp3").toLocalFile();

    ServerStreamer *stream = new ServerStreamer;
    MainWindow w(stream);

    w.show();

    return a.exec();
}
// keep britney sp-... err.. main aloooone

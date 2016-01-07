#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include "serverstreamer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(ServerStreamer *stream, QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pingCLientsButton_clicked();
    void updateClientCount(int);
    void on_startStream_clicked();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    ServerStreamer *stream;
};

#endif // MAIN_WINDOW_H

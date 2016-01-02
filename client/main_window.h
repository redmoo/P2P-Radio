#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include "streamreceiver.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(StreamReceiver *recv, QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_receiveButton_clicked();

private:
    Ui::MainWindow *ui;
    StreamReceiver *receiver;
};

#endif // MAIN_WINDOW_H

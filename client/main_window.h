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

public slots:
    void updateMessageDisplay(QString message);
    void updateConnectionStatusDisplay(QString status);
    void updateActivityLogDisplay(QString activity);
    void updateConnectButton(bool toggle);

private slots:
    void on_receiveButton_clicked();

    void on_verticalSlider_valueChanged(int value);

private:
    Ui::MainWindow *ui;
    StreamReceiver *receiver;
};

#endif // MAIN_WINDOW_H

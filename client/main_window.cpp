#include "main_window.h"
#include "ui_main_window.h"

MainWindow::MainWindow(StreamReceiver *recv, QWidget *parent) :
    receiver(recv),
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("P2P Client");

    receiver->init();

    //reciever.doConnect();
    //reciever.requestNewFortune();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_receiveButton_clicked()
{
    ui->receiveButton->setEnabled(false);
    receiver->newConnect();
}

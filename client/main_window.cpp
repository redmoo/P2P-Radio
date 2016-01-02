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

    connect(receiver, &StreamReceiver::messageChanged, this, &MainWindow::updateMessageDisplay);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateMessageDisplay(QString message)
{
    ui->messageDisplay->setText(message);
}

void MainWindow::on_receiveButton_clicked()
{
    ui->receiveButton->setEnabled(false);
    ui->connectionStatus->setText("Connecting to server...");
    receiver->newConnect();
}

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

    connect(receiver, &StreamReceiver::messageChanged, this, &MainWindow::updateMessageDisplay);
    connect(receiver, &StreamReceiver::connectionStatusChanged, this, &MainWindow::updateConnectionStatusDisplay);
    connect(receiver, &StreamReceiver::activityLogChanged, this, &MainWindow::updateActivityLogDisplay);
    connect(receiver, &StreamReceiver::connectButtonToggle, this, &MainWindow::updateConnectButton);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateMessageDisplay(QString message)
{
    ui->messageDisplay->setText(message);
}

void MainWindow::updateConnectionStatusDisplay(QString status)
{
    ui->connectionStatus->setText(status);
}

void MainWindow::updateActivityLogDisplay(QString activity)
{
    ui->activityDisplay->appendPlainText("--- " + activity + "\r\n");
}

void MainWindow::updateConnectButton(bool toggle)
{
    ui->receiveButton->setEnabled(toggle);
}

void MainWindow::on_receiveButton_clicked()
{
    ui->receiveButton->setEnabled(false);
    //ui->connectionStatus->setText("Establishing connection to server...");
    ui->serverConnectionInfo->setEnabled(false);
    ui->clientConnectionInfo->setEnabled(false);
    receiver->newConnect(ui->serverConnectionInfo->text(), ui->clientConnectionInfo->text());
}

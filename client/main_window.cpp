#include "main_window.h"
#include "ui_main_window.h"

MainWindow::MainWindow(StreamReceiver *recv, QWidget *parent) :
    receiver(recv),
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("P2P Client");

    connect(receiver, &StreamReceiver::connectionInfoChanged, this, &MainWindow::updateClientConnectionInfo);
    connect(receiver, &StreamReceiver::messageChanged, this, &MainWindow::updateMessageDisplay);
    connect(receiver, &StreamReceiver::connectionStatusChanged, this, &MainWindow::updateConnectionStatusDisplay);
    connect(receiver, &StreamReceiver::activityLogChanged, this, &MainWindow::updateActivityLogDisplay);
    connect(receiver, &StreamReceiver::connectButtonToggle, this, &MainWindow::updateConnectButton);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateClientConnectionInfo(QString ip, quint16 port)
{
    ui->clientConnectionInfo->setText(ip);
    ui->clientUdpPortLine->setText(QString::number(port));
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
    ui->serverConnectionInfo->setEnabled(toggle);
    ui->serverTcpPortLine->setEnabled(toggle);
    ui->clientConnectionInfo->setEnabled(toggle);
    ui->clientUdpPortLine->setEnabled(toggle);
}

void MainWindow::on_receiveButton_clicked()
{
    ui->receiveButton->setEnabled(false);
    ui->serverConnectionInfo->setEnabled(false);
    ui->serverTcpPortLine->setEnabled(false);
    ui->clientConnectionInfo->setEnabled(false);
    ui->clientUdpPortLine->setEnabled(false);

    receiver->newConnect(ui->serverConnectionInfo->text(), ui->serverTcpPortLine->text(),
                         ui->clientConnectionInfo->text(), ui->clientUdpPortLine->text());
}

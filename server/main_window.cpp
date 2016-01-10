#include "main_window.h"
#include "ui_main_window.h"

#include <QFileDialog>


MainWindow::MainWindow(ServerStreamer *s, QWidget *parent) :
    stream(s),
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("P2P Master");
    ui->startStream->setEnabled(false);

    connect(stream, &ServerStreamer::connectionInfoChanged, this, &MainWindow::updateConnectionInfo);
    connect(stream, &ServerStreamer::clientCountChanged, this, &MainWindow::updateClientCount);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pingClientsButton_clicked()
{
    stream->sendMessage();
}

void MainWindow::updateClientCount(int count)
{
    ui->numConnections->setText(QString::number(count));
    ui->pingClientsButton->setEnabled(count > 0);
}

void MainWindow::updateConnectionInfo(QString ip, quint16 port)
{
    ui->connectionInfoLine->setText(ip);
    ui->portInfoLine->setText(QString::number(port));
}

void MainWindow::on_startStream_clicked()
{
    ui->startStream->setEnabled(false);
    ui->chainCheckBox->setEnabled(false);
    ui->connectionInfoLine->setEnabled(false);
    ui->portInfoLine->setEnabled(false);
    ui->fileButton->setEnabled(false);

    stream->startStream(ui->connectionInfoLine->text(), ui->portInfoLine->text(), ui->chainCheckBox->isChecked());
}

void MainWindow::on_fileButton_clicked()
{
    QString file_name = QFileDialog::getOpenFileName(this, tr("Open mp3 File"), "/home", "Audio (*.mp3)");
    ui->fileLabel->setText(QFileInfo(file_name).fileName());
    stream->setMusic(QDir::toNativeSeparators(file_name));
    ui->startStream->setEnabled(true);
}

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

    stream->init();

    connect(stream, &ServerStreamer::clientCountChanged, this, &MainWindow::updateClientCount);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pingCLientsButton_clicked()
{
    stream->sendMessage();
}

void MainWindow::updateClientCount(int count)
{
    ui->numConnections->setText(QString::number(count));
}

void MainWindow::on_startStream_clicked()
{
    stream->startStream();
}

void MainWindow::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open mp3 File"), "/home", "Audio (*.mp3)");
    QFileInfo fileInfo(fileName);
    QString fName(fileInfo.fileName());
    ui->fileName->setText(" " + fName);
    stream->audioFile = QDir::toNativeSeparators(fileName);
    ui->startStream->setEnabled(true);

}



#include "main_window.h"
#include "ui_main_window.h"


MainWindow::MainWindow(ServerStreamer *s, QWidget *parent) :
    stream(s),
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("P2P Master");

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
    ui->startStream->setEnabled(false);
    ui->chainCheckBox->setEnabled(false);
    ui->connectionInfoLine->setEnabled(false);
    stream->startStream(ui->connectionInfoLine->text(), ui->chainCheckBox->isChecked());
}

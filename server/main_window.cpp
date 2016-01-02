#include "main_window.h"
#include "ui_main_window.h"


MainWindow::MainWindow(ServerStreamer *s, QWidget *parent) :
    stream(s),
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("P2P Master");

    stream->init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

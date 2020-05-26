#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

void MainWindow::on_connect_button_clicked()
{
    socket.connectToServer("\\\\.\\pipe\\ICS0025",QIODevice::ReadWrite);
    if(socket.waitForConnected(5000)) {
        qDebug("Connected");
    }
    else {
        qDebug(socket.errorString().toUtf8());
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}



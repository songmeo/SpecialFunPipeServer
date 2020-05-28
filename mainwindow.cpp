#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_connect_button_clicked()
{
    socket.connectToServer("\\\\.\\pipe\\ICS0025",QIODevice::ReadWrite);
    if(socket.waitForConnected(5000)) {
        setMsg("Connected");
    }
    else {
        setMsg(socket.errorString().toUtf8());
        return;
    }
    sendMsg();
}

void MainWindow::setMsg(const QString &iconText)
{
    ui->msg->setText(iconText);
}

void MainWindow::sendMsg() {
    QByteArray msg;
    QDataStream s(&msg, QIODevice::WriteOnly);

    QByteArray size;
    QDataStream size_str(&size, QIODevice::WriteOnly);
    size_str.setByteOrder(QDataStream::LittleEndian);

    QString f = ui->function_input->currentText();
    double x0 = ui->x0_input->toPlainText().toDouble();
    double xn = ui->xn_input->toPlainText().toDouble();
    int points = ui->points_input->toPlainText().toInt();
    int order = ui->order_input->toPlainText().toInt();

    s << f << x0 << xn << points << order;
    size_str << msg.size();
    msg.replace(0,4,size);

    socket.write(msg);
}

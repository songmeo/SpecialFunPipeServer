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

    //function
    QString f = ui->function_input->currentText();
    s << f.toUtf8().QByteArray::toHex();
    //msg.append(f.toUtf8().QByteArray::toHex());

    //x0
    double x0 = ui->x0_input->toPlainText().toDouble();
    s << x0;
    //msg.append(x0);

    //xn
    double xn = ui->xn_input->toPlainText().toDouble();
    s << xn;
    //msg.append(xn);

    //points
    int points = ui->points_input->toPlainText().toInt();
    s << points;
    //msg.append(points);

    //order
    int order = ui->order_input->toPlainText().toInt();
    s << order;
    //msg.append(order);

    //size
    //QString size = QString::number(msg.size());
    int size = msg.size();
    msg.prepend(4, 0);
    msg.insert(size, 0);

    socket.write(msg);
}

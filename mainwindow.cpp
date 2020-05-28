#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::hexprint(const QByteArray& arr) {
    unsigned char chars[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
    QString s;
    size_t size = arr.size();
    unsigned char c;
    for(size_t i = 0; i < size; i++) {
        c = arr.at(i);
        s.append(chars[c >> 4]);
        s.append(chars[c & 0xf]);
    }
    setMsg(s);
}


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
    QDataStream msg_str(&msg, QIODevice::WriteOnly);

    QByteArray size;
    QDataStream size_str(&size, QIODevice::WriteOnly);
    msg_str.setByteOrder(QDataStream::LittleEndian);
    size_str.setByteOrder(QDataStream::LittleEndian);
    // msg format:   uint32 size || utf16 f_name || double x0 || double xn || uint32 nPoints ( || order ( bessel only) )
    QString f = ui->function_input->currentText();
    f.append(QChar(0)); //null terminator
    double x0 = ui->x0_input->toPlainText().toDouble();
    double xn = ui->xn_input->toPlainText().toDouble();
    int points = ui->points_input->toPlainText().toInt();
    int order = ui->order_input->toPlainText().toInt();

    msg_str << f << x0 << xn << points << order;
    size_str << msg.size();
    msg.replace(0,4,size);

    hexprint(msg);

    socket.write(msg);
}

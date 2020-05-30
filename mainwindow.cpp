#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qcustomplot.h"
#include <iostream>

//void MainWindow::hexprint(const QByteArray& arr) {
//    unsigned char chars[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
//    QString s;
//    size_t size = arr.size();
//    unsigned char c;
//    for(size_t i = 0; i < size; i++) {
//        c = arr.at(i);
//        s.append(chars[c >> 4]);
//        s.append(chars[c & 0xf]);
//    }
//    setMsg(s);
//}

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

    QValidator* i = new QIntValidator();
    QValidator* d = new QDoubleValidator();
    ui->x0_input->setValidator(d);
    ui->xn_input->setValidator(d);
    ui->points_input->setValidator(i);
    ui->order_input->setValidator(i);
    double x0 = ui->x0_input->text().toDouble();
    double xn = ui->xn_input->text().toDouble();
    int points = ui->points_input->text().toInt();
    int order = ui->order_input->text().toInt();

    msg_str << f << x0 << xn << points << order;
    size_str << msg.size();
    msg.replace(0,4,size);
    //hexprint(msg);
    socket.write(msg);
}

void MainWindow::readMsg() {
    QByteArray bA = socket.readAll();
    bA.remove(0,4);
    QDataStream data(&bA, QIODevice::ReadWrite);
    data.setByteOrder(QDataStream::LittleEndian);
    QString c("Curve");
    QByteArray curve((const char *)c.utf16(), (c.size()+1) * 2);
    if(!bA.contains(curve)) {
        QString error_msg = QString::fromUtf16(reinterpret_cast<const ushort*>(bA.data()), bA.size());
        setMsg(error_msg);
    }
    else {
        QVector<double> x, y;
        data.skipRawData(12); //skip curve
        while(!data.atEnd()) {
            double x_v, y_v;
            data >> x_v >> y_v;
            x.push_back(x_v);
            y.push_back(y_v);
        }
//        for(int i = 0; i < x.size(); i++) {
//            std::cout << "[x=(" << x.at(i) << ") y=(" << y.at(i) << ")]\n";
//        }
        makePlot(x,y);
    }
}

void MainWindow::makePlot(QVector<double> x, QVector<double> y)
{
    ui->graph_view->addGraph();
    ui->graph_view->graph(0)->setData(x,y);
    ui->graph_view->rescaleAxes();
    ui->graph_view->replot();
}

void MainWindow::on_compute_button_clicked()
{
    ui->msg->clear();
    sendMsg();
    if(socket.state() == QLocalSocket::ConnectedState || socket.waitForReadyRead(5000)) {
        readMsg();
    }
    else {
        setMsg("nothing to read");
    }
}

void MainWindow::on_break_button_clicked()
{
    socket.disconnectFromServer();
    if(socket.state() == QLocalSocket::UnconnectedState || socket.waitForDisconnected(5000)) {
        setMsg("Disconnected");
    }
}

void MainWindow::on_exit_button_clicked()
{
    QCoreApplication::exit();
}

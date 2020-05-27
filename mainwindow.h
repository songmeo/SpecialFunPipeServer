#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLocalSocket>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QLocalSocket socket;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_connect_button_clicked();
    void setMsg(const QString &iconText);
    void sendMsg();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H

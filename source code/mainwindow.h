#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_menu_host_ip_triggered();

    void on_menu_service_port_triggered();

    void on_menu_clear_triggered();

public slots:
    void display_output(const QString & result);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

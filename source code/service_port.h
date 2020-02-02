#ifndef SERVICE_PORT_H
#define SERVICE_PORT_H

#define _WINSOCK_DEPRECATE_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")

#include <QDialog>
#include <iostream>
#include <winsock2.h>


using namespace std;
namespace Ui {
class Service_Port;
}

class Service_Port : public QDialog
{
    Q_OBJECT

public:
    explicit Service_Port(QWidget *parent = nullptr);
    ~Service_Port();
    QString get_service_lookup();
    QString get_port_lookup();
    QString get_protocol_lookup();


private slots:
    void on_rb_service_clicked();

    void on_rb_port_clicked();

    void on_btn_ok_sp_p_clicked();

    void on_btn_cancel_sp_p_clicked();

private:
    Ui::Service_Port *ui;

    bool isService = true;

    void resolved_service(char*, char*);
    void resolved_port(char*, char*);

    char* qstring_chararr(QString);

    QString service_lookup;
    QString protocol_lookup;
    QString port_lookup;

    void set_error(QString);

};

#endif // SERVICE_PORT_H

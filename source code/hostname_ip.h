#ifndef HOSTNAME_IP_H
#define HOSTNAME_IP_H

#define _WINSOCK_DEPRECATE_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")
#include <QDialog>
#include <QObject>
#include <iostream>
#include <stdio.h>
#include <string>
#include <winsock2.h>
#include "mainwindow.h"

using namespace std;

namespace Ui {
class HostName_IP;
}

class HostName_IP : public QDialog
{
    Q_OBJECT

public:
    explicit HostName_IP(QWidget *parent = nullptr);
    ~HostName_IP();
    QString get_ip_lookup();
    QString get_hostname_lookup();
    QString get_alias_lookup();

private slots:
    void on_rb_host_clicked();

    void on_rb_ip_clicked();

    void on_btn_ok_hn_ip_clicked();

    void on_btn_cancel_hn_ip_clicked();

private:
    Ui::HostName_IP *ui;
    bool isHost = true;
    void resolved_ip(char*);
    void resolved_host(char*);

    char* qstring_chararr(QString);

    QString ip_lookup;
    QString hostname_lookup;
    QString alias_lookup;

    void set_error(QString);
};


#endif // HOSTNAME_IP_H

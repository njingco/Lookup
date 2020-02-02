/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: mainwindow.cpp
--
-- PROGRAM: lookup
--
-- FUNCTIONS:
--              - MainWindow(QWidget *parent)
--              - ~MainWindow()
--              - MainWindow* get_mainwindow()
--              - void on_menu_host_ip_triggered()
--              - void on_menu_service_port_triggered()
--              - void on_menu_clear_triggered()
--              - void display_output(const QString & result)
--
--
-- DATE: January 19, 2020
--
-- REVISIONS:
--
-- DESIGNER: Nicole Jingco
--
-- PROGRAMMER: Nicole Jingco
--
-- NOTES:
-- This file will handle the main window UI and handle the menu item events and displays the outputs on the window.
----------------------------------------------------------------------------------------------------------------------*/
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "hostname_ip.h"
#include "service_port.h"

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: MainWindow
--
-- DATE: January 17, 2020
--
-- REVISIONS: NA
--
-- DESIGNER: Nicole Jingco
--
-- PROGRAMMER: Nicole Jingco
--
-- INTERFACE: MainWindow(QWidget *parent)
--
-- RETURNS: NA
--
-- NOTES:
-- Cretes the main window
----------------------------------------------------------------------------------------------------------------------*/

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Lookup");
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: :~MainWindow
--
-- DATE: January 17, 2020
--
-- REVISIONS: NA
--
-- DESIGNER: Nicole Jingco
--
-- PROGRAMMER: Nicole Jingco
--
-- INTERFACE: NA
--
-- RETURNS: NA
--
-- NOTES:
-- Destructor of the MainWindow
----------------------------------------------------------------------------------------------------------------------*/
MainWindow::~MainWindow()
{
    delete ui;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: on_menu_host_ip_triggered
--
-- DATE: January 17, 2020
--
-- REVISIONS: NA
--
-- DESIGNER: Nicole Jingco
--
-- PROGRAMMER: Nicole Jingco
--
-- INTERFACE: NA
--
-- RETURNS: void
--
-- NOTES:
-- Handles the hostname/ip ui and displays the output to the main output box
----------------------------------------------------------------------------------------------------------------------*/
void MainWindow::on_menu_host_ip_triggered()
{
    HostName_IP host_ip;
    host_ip.setModal(true);
    if (host_ip.exec() == QDialog::Accepted) {
        QString ip = host_ip.get_ip_lookup();
        QString host = host_ip.get_hostname_lookup();
        QString alias = host_ip.get_alias_lookup();

        QString print = "IP: " + ip + '\n' +
                        "Host Name: " + host + '\n' +
                        "Alias: " + alias + '\n\n';
        ui->tb_output->append(print);
    }
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: on_menu_service_port_triggered
--
-- DATE: January 17, 2020
--
-- REVISIONS: NA
--
-- DESIGNER: Nicole Jingco
--
-- PROGRAMMER: Nicole Jingco
--
-- INTERFACE: NA
--
-- RETURNS: void
--
-- NOTES:
-- Handles the service/port ui and displays the output to the main output box
----------------------------------------------------------------------------------------------------------------------*/
void MainWindow::on_menu_service_port_triggered()
{
    Service_Port service_port;
    service_port.setModal(true);
    if (service_port.exec() == QDialog::Accepted){
        QString service = service_port.get_service_lookup();
        QString protocol = service_port.get_protocol_lookup();
        QString port = service_port.get_port_lookup();

        QString print = "Service: " + service + '\n' +
                        "Protocol: " + protocol + '\n' +
                        "Port: " + port + '\n\n';
        ui->tb_output->append(print);
    }
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: display_output
--
-- DATE: January 17, 2020
--
-- REVISIONS:
--
-- DESIGNER: Nicole Jingco
--
-- PROGRAMMER: Nicole Jingco
--
-- INTERFACE: display_output(const QString & result)
--                      - const QString & result - takes in a QString of the results
--
-- RETURNS: void
--
-- NOTES:
-- Displays the results the main output box
----------------------------------------------------------------------------------------------------------------------*/
void MainWindow::display_output(const QString & result){
    ui->tb_output->append(result);
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: on_menu_clear_triggered
--
-- DATE: January 17, 2020
--
-- REVISIONS: NA
--
-- DESIGNER: Nicole Jingco
--
-- PROGRAMMER: Nicole Jingco
--
-- INTERFACE: NA
--
-- RETURNS: void
--
-- NOTES:
-- Clears the main output box
----------------------------------------------------------------------------------------------------------------------*/
void MainWindow::on_menu_clear_triggered()
{
    ui->tb_output->clear();
}

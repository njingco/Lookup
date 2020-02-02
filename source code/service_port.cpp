/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: service_port.cpp - an class that handles service to port, and port to service resolution
--
-- PROGRAM: lookup
--
-- FUNCTIONS:
--              - QString get_service_lookup();
--              - QString get_port_lookup();
--              - QString get_protocol_lookup();
--              - void on_rb_service_clicked();
--              - void on_rb_port_clicked();
--              - void on_btn_ok_sp_p_clicked();
--              - void on_btn_cancel_sp_p_clicked();
--              - char* resolved_service(char*, char*);
--              - char* resolved_port(char*, char*);
--              - char* qstring_chararr(QString);
--              - QString service_lookup;
--              - QString protocol_lookup;
--              - QString port_lookup;
--              - void set_error(QString);
--
-- DATE: January 18, 2020
--
-- REVISIONS: None
--
-- DESIGNER: Nicole Jingco
--
-- PROGRAMMER: Nicole Jingco
--
-- NOTES:
-- This program will handle the service/port inputs and resolves to its corresponding lookup using the Winsock 2 API
----------------------------------------------------------------------------------------------------------------------*/

#pragma comment(lib, "Ws2_32.lib")
#include "service_port.h"
#include "ui_service_port.h"
#include <winsock2.h>

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: Service_Port
--
-- DATE: January 18, 2020
--
-- REVISIONS:
--
-- DESIGNER: Nicole Jingco
--
-- PROGRAMMER: Nicole Jingco
--
-- INTERFACE: Service_Port(QWidget *parent)
--                      QWidget * parent - Parent window of the popup box
--
-- RETURNS: NA
--
-- NOTES:
-- Creates the UI of the Server/Port popup box
----------------------------------------------------------------------------------------------------------------------*/
Service_Port::Service_Port(QWidget *parent) : QDialog(parent),
                                              ui(new Ui::Service_Port)
{
    ui->setupUi(this);
    this->setWindowTitle("Service/Port");
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: ~Service_Port
--
-- DATE: January 18, 2020
--
-- REVISIONS:
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
-- Destructor of the Service_Port
----------------------------------------------------------------------------------------------------------------------*/
Service_Port::~Service_Port()
{
    delete ui;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: on_rb_service_clicked
--
-- DATE: January 18, 2020
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
-- This function handles the click of the service radiobutton. It will enable the service input box and disable the
-- Port input box. This will also set the isService boolean to true
----------------------------------------------------------------------------------------------------------------------*/
void Service_Port::on_rb_service_clicked()
{
    ui->le_service->setEnabled(true);
    ui->le_port->setEnabled(false);

    ui->le_service->clear();
    ui->le_protocol->clear();
    ui->le_port->clear();
    isService = true;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: on_rb_port_clicked
--
-- DATE: January 18, 2020
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
-- This function handles the click of the port radiobutton. It will enable the port input box and disable the
-- service input box. This will also set the isService boolean to false
----------------------------------------------------------------------------------------------------------------------*/
void Service_Port::on_rb_port_clicked()
{
    ui->le_service->setEnabled(false);
    ui->le_port->setEnabled(true);

    ui->le_service->clear();
    ui->le_protocol->clear();
    ui->le_port->clear();
    isService = false;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: on_btn_ok_sp_p_clicked
--
-- DATE: January 19, 2020
--
-- REVISIONS: Na
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
-- This will handle the Okay Button events. It will check first is its in service mode or not, then check the input
-- boxes if its filled. If its filled it will run the designated function (service/port). If the box is empty, it will
-- display an error message.
----------------------------------------------------------------------------------------------------------------------*/
void Service_Port::on_btn_ok_sp_p_clicked()
{
    QLineEdit *le_service = ui->le_service;
    QLineEdit *le_protocol = ui->le_protocol;
    QLineEdit *le_port = ui->le_port;

    if (isService)
    {
        if (le_service->text().size() == 0)
        {
            set_error("Empty Service Box");
        }
        else
        {
            if (le_protocol->text().size() == 0)
            {
                set_error("Empty Protocol Box");
            }
            else
            {
                resolved_service(qstring_chararr(le_service->text()), qstring_chararr(le_protocol->text()));
            }
        }
    }
    else
    {
        if (le_port->text().size() == 0)
        {
            set_error("Empty Port Box");
        }
        else
        {
            if (le_protocol->text().size() == 0)
            {
                set_error("Empty Protocol Box");
            }
            else
            {
                resolved_port(qstring_chararr(le_port->text()), qstring_chararr(le_protocol->text()));
            }
        }
    }
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: on_btn_cancel_sp_p_clicked
--
-- DATE: January 19, 2020
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
-- This function will handle the cancel function and close the popup box.
----------------------------------------------------------------------------------------------------------------------*/
void Service_Port::on_btn_cancel_sp_p_clicked()
{
    reject();
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: resolved_service
--
-- DATE: January 19, 2020
--
-- REVISIONS: NA
--
-- DESIGNER: Nicole Jingco
--
-- PROGRAMMER: Nicole Jingco
--
-- INTERFACE: resolved_service(char *service, char *protocol)
--                      char *service   - pointer to the first character of an array of the service
--                      char *protocol  - pointer to the first character of an array of the protocol
--
-- RETURNS: void
--
-- NOTES:
-- This fucntion takes find the port using the service and protocol and stores the values in the service_lookup,
-- protocol_lookup and port_lookup if the service is found, then send the accept signal back to the main window.
-- If it is not found it will display and error message.
----------------------------------------------------------------------------------------------------------------------*/
void Service_Port::resolved_service(char *service, char *protocol)
{
    struct servent *sv;
    WORD wVersionRequested = MAKEWORD(2, 2);
    WSADATA wsaData;

    // Open up a Winsock v2.2 session
    WSAStartup(wVersionRequested, &wsaData);

    sv = getservbyname(service, protocol);
    if (sv == NULL)
    {
        set_error("Error in getservbyname");
    }
    else
    {
        service_lookup = service;
        protocol_lookup = protocol;
        port_lookup = QString::number(ntohs(sv->s_port));
        accept();
    }
    WSACleanup();
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: resolved_port
--
-- DATE: January 19, 2020
--
-- REVISIONS: NA
--
-- DESIGNER: Nicole Jingc2o
--
-- PROGRAMMER: Nicole Jingco
--
-- INTERFACE: resolved_port(char *port, char *protocol)
--                      char *port   - pointer to the first character of an array of the port
--                      char *protocol  - pointer to the first character of an array of the protocol
--
-- RETURNS: void
--
-- NOTES:
-- This fucntion takes find the service using the port and protocol and stores the values in the service_lookup,
-- protocol_lookup and port_lookup if the service is found, then send the accept signal back to the main window.
-- If it is not found it will display and error message.
----------------------------------------------------------------------------------------------------------------------*/
void Service_Port::resolved_port(char *port, char *protocol)
{
    struct servent *sv;
    int s_port;

    WORD wVersionRequested = MAKEWORD(2, 2);
    WSADATA wsaData;

    // Open up a Winsock v2.2 session
    WSAStartup(wVersionRequested, &wsaData);

    s_port = atoi(port);
    sv = getservbyport(htons(s_port), protocol);

    if (sv == NULL)
    {
        set_error("Error in getservbyport");
    }
    else
    {
        service_lookup = sv->s_name;
        protocol_lookup = protocol;
        port_lookup = QString::number(s_port);
        accept();
    }
    WSACleanup();
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: qstring_chararr
--
-- DATE: January 19, 2020
--
-- REVISIONS: NA
--
-- DESIGNER: Nicole Jingco
--
-- PROGRAMMER: Nicole Jingco
--
-- INTERFACE: qstring_chararr(QString item)
--                      QString item - takes in a Qstring retrived from the input box
--
-- RETURNS: Return a pointer to the first char in an array of the converted QString
--
-- NOTES:
-- This function converts a QString item to a char array and returns the pointer to the first item in the char array
----------------------------------------------------------------------------------------------------------------------*/
char *Service_Port::qstring_chararr(QString item)
{
    QByteArray ba = item.toLatin1();
    return ba.data();
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:get_service_lookup
--
-- DATE: January 19, 2020
--
-- REVISIONS: NA
--
-- DESIGNER: Nicole Jingco
--
-- PROGRAMMER: Nicole Jingco
--
-- INTERFACE: NA
--
-- RETURNS: returns the service lookup QString
--
-- NOTES:
-- This returns the service lookup QString
----------------------------------------------------------------------------------------------------------------------*/
QString Service_Port::get_service_lookup()
{
    return service_lookup;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: get_port_lookup
--
-- DATE: January 19, 2020
--
-- REVISIONS: NA
--
-- DESIGNER: Nicole Jingco
--
-- PROGRAMMER: Nicole Jingco
--
-- INTERFACE: NA
--
-- RETURNS: returns the port lookup QString
--
-- NOTES:
-- This returns the port lookup QString
----------------------------------------------------------------------------------------------------------------------*/
QString Service_Port::get_port_lookup()
{
    return port_lookup;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: get_protocol_lookup
--
-- DATE: January 19, 2020
--
-- REVISIONS: NA
--
-- DESIGNER: Nicole Jingco
--
-- PROGRAMMER: Nicole Jingco
--
-- INTERFACE: NA
--
-- RETURNS: returns the protocol lookup QString
--
-- NOTES:
-- This returns the protocol lookup QString
----------------------------------------------------------------------------------------------------------------------*/
QString Service_Port::get_protocol_lookup()
{
    return protocol_lookup;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: set_error
--
-- DATE: January 19, 2020
--
-- REVISIONS: NA
--
-- DESIGNER: Nicole Jingco
--
-- PROGRAMMER: Nicole Jingco
--
-- INTERFACE:set_error(QString msg)
--                  QString msg - takes in a QString of the error message that will be displayed
--
-- RETURNS: void
--
-- NOTES:
-- This function takes in the error message and displays it into the window.
----------------------------------------------------------------------------------------------------------------------*/
void Service_Port::set_error(QString msg)
{
    ui->sp_p_error->setText("Error: " + msg);
}

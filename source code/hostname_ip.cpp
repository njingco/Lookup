/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: hostname_ip.cpp
--
-- PROGRAM: lookup
--
-- FUNCTIONS:
--              - QString get_service_lookup()
--              - QString get_port_lookup()
--              - QString get_protocol_lookup()
--              - void on_rb_service_clicked()
--              - void on_rb_port_clicked()
--              - void on_btn_ok_sp_p_clicked()
--              - void on_btn_cancel_sp_p_clicked()
--              - void resolved_service(char*, char*)
--              - void resolved_port(char*, char*)
--              - char* qstring_chararr(QString)
--              -  void set_error(QString)
--
--
-- DATE: January 17, 2020
--
-- REVISIONS: NA
--
-- DESIGNER: Nicole Jingco
--
-- PROGRAMMER: Nicole Jingco
--
-- NOTES:
-- This program will handle the host name/ip inputs and resolves to its corresponding lookup using the Winsock 2 API
----------------------------------------------------------------------------------------------------------------------*/
#include "hostname_ip.h"
#include "ui_hostname_ip.h"

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: HostName_IP
--
-- DATE: January 17, 2020
--
-- REVISIONS:
--
-- DESIGNER: Nicole Jingco
--
-- PROGRAMMER: Nicole Jingco
--
-- INTERFACE: HostName_IP(QWidget *parent)
--                      QWidget * parent - Parent window of the popup box
-- RETURNS:
--
-- NOTES:
-- Creates the UI of the Host Name/IP popup box
----------------------------------------------------------------------------------------------------------------------*/
HostName_IP::HostName_IP(QWidget *parent) : QDialog(parent),
                                            ui(new Ui::HostName_IP)
{
    ui->setupUi(this);
    this->setWindowTitle("Host/IP");
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:~HostName_IP
--
-- DATE: January 17, 2020
--
-- REVISIONS: NA
--
-- DESIGNER: Nicole Jingco
--
-- PROGRAMMER: Nicole Jingco
--
-- INTERFACE:NA
--
-- RETURNS: NA
--
-- NOTES:
-- Destructor of the HostName_IP
----------------------------------------------------------------------------------------------------------------------*/
HostName_IP::~HostName_IP()
{
    delete ui;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: on_rb_host_clicked
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
-- This function handles the click of the hostname radiobutton. It will enable the host name input box and disable the
-- ip input box. This will also set the isHost boolean to true
----------------------------------------------------------------------------------------------------------------------*/
void HostName_IP::on_rb_host_clicked()
{
    ui->le_ip->setEnabled(false);
    ui->le_host->setEnabled(true);
    ui->le_ip->clear();
    ui->le_host->clear();
    isHost = true;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: on_rb_ip_clicked
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
-- This function handles the click of the ip radiobutton. It will enable the ip input box and disable the
-- host name input box. This will also set the isHost boolean to false
----------------------------------------------------------------------------------------------------------------------*/
void HostName_IP::on_rb_ip_clicked()
{
    ui->le_ip->setEnabled(true);
    ui->le_host->setEnabled(false);
    ui->le_ip->clear();
    ui->le_host->clear();
    isHost = false;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: on_btn_ok_hn_ip_clicked
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
-- This will handle the Okay Button events. It will check first is its in host mode or not, then check the input
-- boxes if its filled. If its filled it will run the designated function (hostname/ip). If the box is empty, it will
-- display an error message.
----------------------------------------------------------------------------------------------------------------------*/
void HostName_IP::on_btn_ok_hn_ip_clicked()
{
    QLineEdit *le_host = ui->le_host;
    QLineEdit *le_ip = ui->le_ip;

    if (isHost)
    {
        if (le_host->text().size() == 0)
        {
            set_error("Host Name Box Empty");
        }
        else
        {
            resolved_host(qstring_chararr(le_host->text()));
        }
    }
    else
    {
        if (le_ip->text().size() == 0)
        {
            set_error("IP Box Empty");
        }
        else
        {
            resolved_ip(qstring_chararr(le_ip->text()));
        }
    }
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: on_btn_cancel_hn_ip_clicked
--
-- DATE: January 17, 2020
--
-- REVISIONS: NA
--
-- DESIGNER: Nicole Jingco
--
-- PROGRAMMER: Nicole Jingco
--
-- INTERFACE:
--
-- RETURNS:
--
-- NOTES:
-- This function will handle the cancel function and close the popup box.
----------------------------------------------------------------------------------------------------------------------*/
void HostName_IP::on_btn_cancel_hn_ip_clicked()
{
    reject();
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: resolved_ip
--
-- DATE: January 17, 2020
--
-- REVISIONS: NA
--
-- DESIGNER: Nicole Jingco
--
-- PROGRAMMER: Nicole Jingco
--
-- INTERFACE: resolved_ip(char *ip)
--                      char *ip - pointer to the first character of an array of the ip
--
-- RETURNS: void
--
-- NOTES:
-- This fucntion takes find the host name using the ip and stores the values in the hostname_lookup,
-- get_alias_lookup, and ip_lookup if the hostname is found, then send the accept signal back to the main window.
-- If it is not found it will display and error message.
----------------------------------------------------------------------------------------------------------------------*/
void HostName_IP::resolved_ip(char *ip)
{
    int a;
    char **p;
    struct hostent *hp;
    struct in_addr my_addr, *addr_p;
    char ip_address[256];

    WORD wVersionRequested = MAKEWORD(2, 2);
    WSADATA wsaData;

    // Open up a Winsock v2.2 session
    WSAStartup(wVersionRequested, &wsaData);

    addr_p = (struct in_addr *)malloc(sizeof(struct in_addr));
    addr_p = &my_addr;

    if (isdigit(*ip)) // Dotted IP?
    {
        if ((a = inet_addr(ip) == 0))
        {
            set_error("IP Address must be of the form x.x.x.x");
        }

        // Copy IP address  into ip_address
        strcpy(ip_address, ip);
        addr_p->s_addr = inet_addr(ip_address);
        hp = gethostbyaddr((char *)addr_p, PF_INET, sizeof(my_addr));

        if (hp == NULL)
        {
            set_error("Host information \nnot found");
        }
        else
        {
            for (p = hp->h_addr_list; *p != 0; p++)
            {
                struct in_addr in;
                char **q;

                memcpy(&in.s_addr, *p, sizeof(in.s_addr));
                ip_lookup = ip;
                hostname_lookup = hp->h_name;
                for (q = hp->h_aliases; *q != 0; q++)
                {
                    alias_lookup = *q;
                }
            }
            accept();
        }
    }
    else
    {
        set_error("Not an IP");
    }

    WSACleanup();
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: resolved_host
--
-- DATE: January 17, 2020
--
-- REVISIONS: NA
--
-- DESIGNER: Nicole Jingco
--
-- PROGRAMMER: Nicole Jingco
--
-- INTERFACE: resolved_host(char *hostname)
--                      char *hostname   - pointer to the first character of an array of the hostname
--
-- RETURNS: void
--
-- NOTES:
-- This fucntion takes find the ip using the host name and stores the values in the hostname_lookup,
-- get_alias_lookup, and ip_lookup if the ip is found, then send the accept signal back to the main window.
-- If it is not found it will display and error message.
----------------------------------------------------------------------------------------------------------------------*/
void HostName_IP::resolved_host(char *hostname)
{
    struct in_addr my_addr, *addr_p;
    char **p;
    struct hostent *hp;

    WORD wVersionRequested = MAKEWORD(2, 2);
    WSADATA wsaData;

    // Open up a Winsock v2.2 session
    WSAStartup(wVersionRequested, &wsaData);

    addr_p = (struct in_addr *)malloc(sizeof(struct in_addr));
    addr_p = &my_addr;

    if (isdigit(*hostname))
    {
        set_error("Not a Host Name");
    }
    else if ((hp = gethostbyname(hostname)) == NULL) // Host name?
    {
        switch (h_errno)
        {
        case HOST_NOT_FOUND:
            set_error("No such host");
            break;
        case TRY_AGAIN:
            set_error("Try again later");
            break;
        case NO_RECOVERY:
            set_error("DNS Error");
            break;
        case NO_ADDRESS:
            set_error("No address");
            break;
        default:
            set_error("Unknown Error");
        }
    }
    else
    {
        for (p = hp->h_addr_list; *p != 0; p++)
        {
            struct in_addr in;
            char **q;

            memcpy(&in.s_addr, *p, sizeof(in.s_addr));
            ip_lookup = inet_ntoa(in);
            hostname_lookup = hp->h_name;
            for (q = hp->h_aliases; *q != 0; q++)
            {
                alias_lookup = *q;
            }
        }
        accept();
    }
    WSACleanup();
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: qstring_chararr
--
-- DATE: January 17, 2020
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
char *HostName_IP::qstring_chararr(QString item)
{
    QByteArray ba = item.toLatin1();
    return ba.data();
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: get_ip_lookup
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
-- RETURNS: returns the ip lookup QString
--
-- NOTES:
-- This returns the ip lookup QString
----------------------------------------------------------------------------------------------------------------------*/
QString HostName_IP::get_ip_lookup()
{
    return ip_lookup;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: get_hostname_lookup
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
-- RETURNS: returns the host name lookup QString
--
-- NOTES:
-- This returns the host name lookup QString
----------------------------------------------------------------------------------------------------------------------*/
QString HostName_IP::get_hostname_lookup()
{
    return hostname_lookup;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: get_alias_lookup
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
-- RETURNS: returns the alias lookup QString
--
-- NOTES:
-- This returns the alias lookup QString
----------------------------------------------------------------------------------------------------------------------*/
QString HostName_IP::get_alias_lookup()
{
    return alias_lookup;
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
void HostName_IP::set_error(QString msg)
{
    ui->host_ip_error->setText("Error: " + msg);
}

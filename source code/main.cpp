/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE:
--
-- PROGRAM:
--
-- FUNCTIONS:
--
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
--
----------------------------------------------------------------------------------------------------------------------*/
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication a(argc, argv);
    MainWindow m;
    m.show();

    return a.exec();
}

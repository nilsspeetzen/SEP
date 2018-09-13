#include "mainwindow.h"
#include "testclass.h"
#include "cascade.h"
#include <QApplication>

/**
 * @file main.cpp
 * @brief main funcion
 * @param argc dummy
 * @param argv dummy
 *
 * \mainpage Description
 * SEP Projekt zu Flashkaskaden
 */

const static int NUMSUBSTANCES = 2;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    cascade C;

    return a.exec();
}

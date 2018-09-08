#include "mainwindow.h"
#include <QApplication>

/**
 * @file main.cpp
 * @brief main funcion
 * @param argc
 * @param argv
 *
 * \mainpage Description
 * SEP Projekt zu Flashkaskaden
 */

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}

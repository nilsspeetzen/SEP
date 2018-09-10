#include "mainwindow.h"
#include "testclass.h"
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

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    TestClass perp;
    w.setProgress(perp.helloTestClass());
    perp.makeNewButton(50, 50, &w);

    return a.exec();
}

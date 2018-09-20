#include "mainwindow.h"
//#include "testclass.h"
#include "cascade.h"
#include <iostream>

#include <QApplication>
//für QML
#include <QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlEngine>

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
    /*
    QApplication a(argc, argv);
    MainWindow w;

    static int NUMSUBSTANCES = 2;
    static Matrix<double,2,7> A;
    A <<    73.649, -7258.2, 0, 0, -7.3037, 4.1653e-6, 2,
            79.276, -10105, 0, 0, -7.521, 7.3408e-19, 6;


    cascade<> C(NUMSUBSTANCES, A);
    C.addFlash();
    C.getFlash(0).pg() = 1000;
    C.getFlash(0).F() = 3;
    QR<> lsol;
    MODULENEWTON<> nlsol(lsol);
    nlsol.eps() = 1;
    nlsol.solve(C.getFlash(0));
    //TODO Matrixdimensionen fixen
    std::cout << "x: " << std::endl << C.getFlash(0).f() << std::endl;

    //w.show();
    */

    // QML TEST:
    QGuiApplication a(argc, argv);

    qmlRegisterType<MainWindow>("mainwindow", 1, 0, "MainWindow");
    QQmlApplicationEngine engine(QUrl(QStringLiteral("qrc:/main.qml")));

    return a.exec();
}

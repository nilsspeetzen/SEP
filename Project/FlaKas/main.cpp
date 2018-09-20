#include "WorldMachine.h"
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
    QGuiApplication a(argc, argv);
    QQmlApplicationEngine engine(QUrl(QStringLiteral("qrc:/main.qml")));
    QObject* root = engine.rootObjects()[0];
    WorldMachine w(root);
    QObject::connect(root, SIGNAL(s_startOneFlash()), &w, SLOT(startOneFlashSlot()));

    return a.exec();
}

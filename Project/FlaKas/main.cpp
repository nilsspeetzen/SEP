#include "WorldMachine.h"
#include <QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlEngine>

/**
 * @file main.cpp
 * @brief Programminitiation
 * @param argc dummy
 * @param argv dummy
 *
 * \mainpage Description
 * SEP Projekt zu Flashkaskaden
 */

int main(int argc, char *argv[])
{
    QGuiApplication a(argc, argv);
    QQmlApplicationEngine* engine = new QQmlApplicationEngine(QUrl(QStringLiteral("qrc:/main.qml")));
    QObject* root = engine->rootObjects()[0];

    WorldMachine w(engine, root);

    QObject::connect(root, SIGNAL(s_startCascade()), &w, SLOT(startCascadeSlot()));
    QObject::connect(root, SIGNAL(s_addFlash(int)), &w, SLOT(addFlashSlot(int)));
    QObject::connect(root, SIGNAL(s_deleteFlash(int)), &w, SLOT(deleteFlashSlot(int)));
    QObject::connect(root, SIGNAL(s_connectFlashes(int, int, int)), &w, SLOT(connectFlashesSlot(int, int, int)));

    return a.exec();
}

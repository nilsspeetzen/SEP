#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore>
#include <QtQml/QQmlApplicationEngine>
#include <QQmlComponent>
#include <QtQuick/QQuickWindow>
#include "cascadesolver.h"


/**
 * @file WorldMachine.h
 * @brief C++ - QML Schnittstelle
 */

/**
 *@brief C++ - QML Schnittstelle
 * Organisiert die Kaskade und das GUI
 */
class WorldMachine : public QObject
{
    Q_OBJECT

private:
    QQmlApplicationEngine* _engine;
    QObject* _root;
	Matrix<double, 2, 7> A;
    cascade<>* C;
    int NUMSUBSTANCES;

    void setUpDataWindow() {
        QQmlComponent newWindow(_engine);
        newWindow.loadUrl(QUrl(QStringLiteral("qrc:/dataWindow.qml")));
        if(newWindow.isReady()) {
            QQuickWindow* dataWindow = dynamic_cast<QQuickWindow*>(newWindow.create(_engine->rootContext()));
            //dataWindow->setParent(_root);
            dataWindow->show();
            //TODO gib Window die DATEN
        }
        else {
            qDebug() << "newWindow not ready";
        }
    }

public:
    WorldMachine(QQmlApplicationEngine* engine, QObject* root) : _engine(engine), _root(root) {
        NUMSUBSTANCES = 2;
        A <<    73.649, -7258.2, 0, 0, -7.3037, 4.1653e-6, 2,
                79.276, -10105, 0, 0, -7.521, 7.3408e-19, 6;
        C = new cascade<>(NUMSUBSTANCES, A);
    }

signals:
public slots:
    void startCascadeSlot() {
        qDebug() << "Starte Löser";
        CascadeSolver solver;
        solver.solve();
        //setUpDataWindow();
    }

    void addFlashSlot(const int& id) {
        qDebug() << "Erstelle: " << id;
        C->addFlash(id);
    }

    void deleteFlashSlot(const int& id) {
        qDebug() << "Lösche: " << id;
        C->deleteFlash(id);
    }

    void connectFlashesSlot(const int& id1, const int& id2, const int& phase) {
        qDebug() << "Verbinde: " << id1 << id2 << phase;
        C->connectFlashes(id1, id2, phase);
    }
};

#endif // MAINWINDOW_H

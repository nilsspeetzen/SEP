#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore>
#include <QtQml/QQmlApplicationEngine>
#include <QQmlComponent>
#include "cascade.h"


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
    QString m_userName;
    QQmlApplicationEngine* _engine;
    QObject* _root;
    int NUMSUBSTANCES;
    Matrix<double,2,7> A;
    cascade<>* C;

public:
    WorldMachine(QQmlApplicationEngine* engine, QObject* root) : _engine(engine), _root(root) {
        NUMSUBSTANCES = 2;
        A <<    73.649, -7258.2, 0, 0, -7.3037, 4.1653e-6, 2,
                79.276, -10105, 0, 0, -7.521, 7.3408e-19, 6;
        C = new cascade<>(NUMSUBSTANCES, A);
    }

signals:

public slots:
    void startOneFlashSlot() {
        QR<> lsol;
        MODULENEWTON<> nlsol(lsol);
        nlsol.eps() = 1;
        nlsol.solve(C->getFlash(0));
        std::stringstream data;
        data << C->getFlash(0).x();
        QString qdata = QString::fromStdString(data.str());
        //std::cout << "x: " << std::endl << data.str() << std::endl;
        //_root->findChild<QObject*>("dataDisplay")->setProperty("text", qdata);
    }

    void addFlashSlot() {
        C->addFlash();
    }

    void deleteFlashSlot(const int& id) {
        qDebug() << id;
    }

    void connectFlashesSlot(const int& id1, const int& id2, const int& phase) {
        qDebug() << id1 << id2 << phase;
    }
};

#endif // MAINWINDOW_H
